from flask import (
    Blueprint, flash, g, redirect, render_template, request, session,url_for
)
from werkzeug.exceptions import abort
import os
import subprocess
from flask_wtf.csrf import CsrfProtect
from flask_wtf.csrf import CSRFError

csrf = CsrfProtect()
from login import login_required
from db import get_db

bp = Blueprint('spell_check', __name__)


@bp.errorhandler(CSRFError)
def handle_csrf_error(e):
    return render_template('error.html', reason=e.description)

@bp.before_request
def check_csrf():
    csrf.protect()
 

@bp.route('/spell_check',methods=['GET','POST'])
@login_required
def spell_check():
    if request.method == 'POST':
        words=["abc","def"]
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
        
        return render_template('/spell.html',textout=suppliedText,mispelled=output)
    else:
        return render_template('/welcome.html')
        #fetch output
        
        #encode output as string 
        
        #sanitize output, remove characters which can cause issues
        
        #send output in form of array to render_template
        
        
    
    
