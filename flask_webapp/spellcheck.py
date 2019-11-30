from flask import (
    Blueprint, flash, g, redirect, render_template, request, session,url_for
)
from werkzeug.exceptions import abort
import os
import subprocess
from flask_wtf.csrf import CsrfProtect
from flask_wtf.csrf import CSRFError
import datetime
csrf = CsrfProtect()
from login import login_required
from db import get_db

bp = Blueprint('spell_check', __name__)


@bp.errorhandler(CSRFError)
def handle_csrf_error(e):
    return render_template('error.html', reason=e.description)

@bp.before_request
def check_csrf():
    
    print("CSRF-protection-CALLED")
    csrf.protect()
 

@bp.route('/logout')
def logout():
    print(request.headers)
    originSplice=str(request.headers)
    #print(originSplice)
    #refererValue=originSplice.split('Referer: ')[1].split('\r')[0]
    #print("LOGOUT",refererValue)
    if request.referrer is not None:
        hostExtract=str(request.referrer).split("http://")[1]
        if hostExtract is not None:
            
            verb=hostExtract.split("/")
            if verb is not None and request.headers.get('host') is not None:
                if str(verb[0])==request.headers.get('host'):
                    print("Session cleared")
                    user_id = session.get('user_id')
                    username= get_db().execute(
            'SELECT * FROM user WHERE id = ?', (user_id,)
        ).fetchone()
                    #print(username['username'])
                    db = get_db()
                    loginsert = 'INSERT INTO logs(userid, request, request_time) VALUES (?, ?, ?)'
                    db.execute(loginsert,(username['username'], 'logout', datetime.datetime.now()))
                    db.commit()
                    session.clear()
                    
    return redirect(url_for('login.welcome'))
        

@bp.route('/spell_check',methods=['GET','POST'])
@login_required
def spell_check():
    if request.method == 'POST':
        
        print(request.headers.get('origin'))

        wordStr = request.form['inputtext']
        
        #Most crucial part
        #Extract input text
        fileName = str(session.get('user_id'))+"-input_file.txt"
        print("DEBUG Obtained",wordStr)
        if os.path.exists(fileName):
            os.remove(fileName)
        else:
            print("New file")
        f = open(fileName, 'w+')
        f.write(wordStr)
        f.close()
        suppliedText = wordStr
        #output = subprocess.check_output(['ls'])
        #print(output)
        #FNULL = open(os.devnull, 'w')
        args = ("./a.out",str(fileName),"dictmain.txt")
        try:
            popen = subprocess.Popen(args, stdout=subprocess.PIPE)
            popen.wait()
            output = popen.stdout.read()
            #print(output)
            #print(type(output))
            output=output.decode().replace("\n",",")
            print("Mispelled words returned as ",output)
        except subprocess.CalledProcessError as e:   
            print("Error :", e)
        db= get_db()
        user_name = g.user['username']
        insertRequest = 'INSERT into queries(userid, query_text, query_response) VALUES (?, ?, ?)'
        db.cursor().execute(insertRequest,(user_name,suppliedText,output))
        db.commit()
        return render_template('/spell.html',textout=suppliedText,mispelled=output)
    else:
        return render_template('/welcome.html')
        #fetch output
        
        #encode output as string 
        
        #sanitize output, remove characters which can cause issues
        
        #send output in form of array to render_template
        
        
@bp.after_request
def apply_caching(response):
    response.headers["X-Frame-Options"] = "SAMEORIGIN"
    response.headers['Content-Security-Policy'] = "default-src 'self'"
    return response   

@bp.before_app_request
def load_logged_in_user():
    user_id = session.get('user_id')

    if user_id is None:
        g.user = None
    else:
        g.user = get_db().execute(
            'SELECT * FROM user WHERE id = ?', (user_id,)
        ).fetchone()

