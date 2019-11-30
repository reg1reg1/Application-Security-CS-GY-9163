import os

from flask import Flask
import login
import db
import spellcheck
import logs
from flask_wtf.csrf import CsrfProtect
from werkzeug.security import generate_password_hash
csrf = CsrfProtect()



def create_app(test_config=None):
    # create and configure the app
    app = Flask(__name__, instance_relative_config=True)
    csrf.init_app(app)
    app.config.from_mapping(
        SECRET_KEY='dev',
        DATABASE=os.path.join(app.instance_path, 'flaskr.sqlite'),
    )
    app.config.update(
    SESSION_COOKIE_HTTPONLY=True,
    SESSION_COOKIE_SAMESITE='Strict',
    )
    if test_config is None:
        # load the instance config, if it exists, when not testing
        app.config.from_pyfile('config.py', silent=True)
        app.config.update(
            PERMANENT_SESSION_LIFETIME=600
        )

    else:
        # load the test config if passed in
        app.config.from_mapping(test_config)

    # ensure the instance folder exists
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    
    
    
    #Registering the database part.
    db.init_app(app)
    
    #These lines will make the database initialize every time the app is run. Comment it out to separate it
    with app.app_context():
        db.init_db()
        #Initializing the first entry with the amdin user
        dbobj = db.get_db()
        #Would be not present in real world application(Password in cleartext being inserted)
        dbobj.execute('INSERT INTO user (username, password,phone, isAdmin) VALUES (?, ?, ?, ?)',('admin', generate_password_hash('Administrator@1'),'12345678901',True))
        dbobj.commit()
    
    app.register_blueprint(login.root_view)
    app.register_blueprint(spellcheck.bp)
    app.register_blueprint(logs.logs_bp)
    csrf.exempt(login.root_view)
    app.add_url_rule('/', endpoint='spellcheck')
    app.add_url_rule('/', endpoint='logs')
    return app


