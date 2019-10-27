 
import functools

from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for
)
from werkzeug.security import check_password_hash, generate_password_hash


from db import get_db

#rootview
root_view = Blueprint('login', __name__, url_prefix='')

@root_view.route('/',methods=['GET'])
def welcome():
    return render_template('/welcome.html')




@root_view.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form['uname']
        password = request.form['pword']
        phone = request.form['2fa']
        print(username,password,phone)
        db = get_db()
        error = None
        
        if not username:
            error = 1
        elif not password:
            error = 1
        elif db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone() is not None:
            error = 'User {} is already registered.'.format(username)
        
        print(error)
        
        #SQLI->SANITIZE
        if error is None:
            db.execute(
                'INSERT INTO user (username, password,phone) VALUES (?, ?, ?)',
                (username, generate_password_hash(password),phone)
            )
            db.commit()
            return render_template('/successful.html')
        else:
            return render_template('/regfail.html')
        flash(error)

    return render_template('/register.html')



@root_view.route('/logout')
def logout():
    session.clear()
    return redirect(url_for('login.welcome'))

@root_view.route('/loginresult')
def loginresult():
    #Sanitize result
    return render_template('/loginok.html',result=request.args.get('result'))
    


@root_view.route('/login', methods=['GET', 'POST'])
def login():
    
    if request.method == 'POST':
        #logged in users should not try to login
        user_id = g.user
        if user_id is not None:
            return redirect(url_for('login.welcome'))
        username = request.form['uname']
        password = request.form['pword']
        twoFactorCode = request.form['2fa']
        
        #Sanitize and encode the received input here:
        print("values received",username,password,twoFactorCode)
        
        db = get_db()
        error = None
        
        #Remove later, chances of sqli
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()
        
        print("values received from the database are as follows",user)
        
        if user is None:
            error = "Incorrect credentials"
        elif not check_password_hash(user['password'], password):
            error = "Incorrect credentials"
        
        elif not user['phone']==twoFactorCode:
            error = "Two-factor authentication failure"
        
        if error is None:
            session.clear()
            session['user_id'] = user['id']
            session.permanent = True
            
        return redirect(url_for('login.loginresult',result=error))
        flash(error)
    else:
        user_id = g.user
        if user_id is not None:
            return redirect(url_for('login.welcome'))
        return render_template('/login.html')        
    

#mapping the session to an existing user in the database.
@root_view.before_app_request
def load_logged_in_user():
    user_id = session.get('user_id')

    if user_id is None:
        g.user = None
    else:
        g.user = get_db().execute(
            'SELECT * FROM user WHERE id = ?', (user_id,)
        ).fetchone()


def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('login.login'))

        return view(**kwargs)

    return wrapped_view

@root_view.after_request
def apply_caching(response):
    response.headers["X-Frame-Options"] = "SAMEORIGIN"
    response.headers['Content-Security-Policy'] = "default-src 'self'"
    return response
