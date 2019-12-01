from flask import (
    Blueprint, flash, g, redirect, render_template, request, session,url_for
)
import functools
from werkzeug.exceptions import abort
import os
import subprocess
from flask_wtf.csrf import CsrfProtect
from flask_wtf.csrf import CSRFError
from login import login_required
csrf = CsrfProtect()
import login
from db import get_db

logs_bp = Blueprint('logs', __name__)

#Enforce access control: 2 tiers of user, admin and non-admin
def admin_login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        #print(g.user['username'])
        if g.user is None:
            return redirect(url_for('login.login'))
        if g.user['username']!='admin':
            return redirect(url_for('login.welcome'))
        return view(**kwargs)

    return wrapped_view




@logs_bp.route('/login_history',methods=['GET','POST'])
@admin_login_required
def logview():
    if request.method == 'POST':
        isValid=True
        #Check whether the user is valid
        dbObj= get_db()
        username = request.form['userid']
        print("Querying this username",username)
        db = dbObj.cursor()
        fetch = db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone()
        if fetch is None:
            isValid=False
            msg = error = 'No such user {}.'.format(username)
            return render_template('/logsout.html',array=msg,valid=isValid)
        
        logQuery = 'SELECT * FROM logs WHERE userid = ?'
        dbOutput=db.execute(logQuery,(username,))
        outputList = list()
        for i in dbOutput:
            outputList.append((i[0],i[1],i[2],i[3]))
        return render_template('/logsout.html',username=username,textout=outputList,valid=isValid)
    else:
        return render_template('/logs.html')

@logs_bp.route('/history',methods=['GET','POST'])
@login_required
def history():
    print("Hello World")
    if request.method == 'GET':
        db = get_db()
        username=g.user['username']
        logQuery = 'SELECT id FROM queries WHERE userid = ?'
        dbOutput=db.cursor().execute(logQuery,(username,))
        isAdmin=False
        isValid=True
        
        #checking if user actually exists in the database , (Session tampering)
        fetch = db.cursor().execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()
        if fetch is None:
            isValid=False
        
        
        outputList = list()

        if g.user['username']=='admin':
            isAdmin=True
        
        if dbOutput:
            for i in dbOutput:
                outputList.append(i[0])
            print(outputList)
        return render_template('/history',textout=outputList,num=len(outputList),isAdmin=isAdmin,isValid=isValid,username="you")
    
    elif request.method == 'POST':
        db = get_db()
        outputList = list()
        if g.user['username']=='admin':
            isValid=True
            username = request.form['userid']
            print("Obtained ",request.form["userid"])
            
            #check whether user is valid
            fetch = db.cursor().execute('SELECT * FROM user WHERE username = ?', (username,)).fetchone()
            if fetch is None:
                isValid=False
                return render_template('/history',textout=[],num=len(outputList),isAdmin=True,isValid=isValid)
            
            #User is valid
            logQuery = 'SELECT id FROM queries WHERE userid = ?'
            dbOutput=db.cursor().execute(logQuery,(username,))
            if dbOutput:
                for i in dbOutput:
                    outputList.append(i[0])
                print(outputList)
            return render_template('/history',textout=outputList,num=len(outputList),isAdmin=True,isValid=True,username=username)
            
        else:
            return redirect(url_for('logs.history'))
        

@logs_bp.route('/history/query<queryId>',methods=['GET'])
@login_required
def query_review(queryId):
    if g.user['username']!='admin':
        madeQueries=False
        validUser=False
        userid=g.user['username']
        db = get_db()
        queryText=""
        queryResponse=""
        if str(queryId).isdigit():
            #normal user is restricted from querying id's outside his userid
            reviewQuery = 'SELECT * FROM queries WHERE userid = ? AND id = ?'
            if userid:
                validUser=True
                dbOutput=db.cursor().execute(reviewQuery,(userid,queryId,))
                
                if dbOutput is not None:
                    for i in dbOutput:
                        queryText= i[2]
                        queryResponse = i[3]
                        madeQueries=True
                
                return render_template('/queryreview.html',queryId=queryId,userid=userid,queryText=queryText,queryResponse=queryResponse,madeQueries=madeQueries)
            return render_template('/invalidid.html',invalidcode=0)
        else:
            return render_template('/invalidid.html',invalidcode=1)
    else:
        madeQueries=False
        validUser=False
        userid=g.user['username']
        db = get_db()
        queryText=""
        queryResponse=""
        if str(queryId).isdigit():
            #Admin can query any queryId as long as it exists in the database.
            reviewQuery = 'SELECT * FROM queries WHERE id = ?'
            if userid:
                validUser=True
                dbOutput=db.cursor().execute(reviewQuery,(queryId,))
            
                if dbOutput is not None:
                    
                    for i in dbOutput:
                        queryText= i[2]
                        userid=i[1]
                        queryResponse = i[3]
                        madeQueries= True
                
                return render_template('/queryreview.html',queryId=queryId,userid=userid,queryText=queryText,queryResponse=queryResponse,madeQueries=madeQueries)
            return render_template('/invalidid.html',invalidcode=0)
        else:
            return render_template('/invalidid.html',invalidcode=1)
