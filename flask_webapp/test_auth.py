import pytest
from flask import g, session
from db import get_db
import re
#this tests the registration and login functionality of the app

def test_register(client, app):
    assert client.get('/register').status_code == 200
    response = client.post(
        '/register', data={'uname': 'a', 'pword': 'a','2fa': '123456'}
    )
    

    with app.app_context():
        assert get_db().execute(
            "select * from user where username = 'a'",
        ).fetchone() is not None


#registration should not be allowed for duplicate userId's
@pytest.mark.parametrize(('username', 'password', 'phone', 'message'), (
    ('reg1', 'test','1234567890', b'success'),
    ('reg1', 'tes1','12367890', b'failure')
))
def test_register_validate_input(client, username, password, phone, message):
    response = client.post(
        '/register',
        data={'uname': username, 'pword': password, '2fa':phone}
    )
    assert b"id=\"success\"" in response.data
    assert message in response.data


def test_login(client, auth):
    #assert client.get('/login').status_code == 200
    response = auth.login()
    #print(response.data)
    with client:
        #logged in clients must be redirected away from  login page
        assert client.get('/login').status_code==302
        assert client.get('/spell_check').status_code==200
        assert session['user_id'] == 2
        assert g.user['username'] == 'reg1'


#check spelling working
def test_spell_check_page(client,auth):
    response = auth.login()
    
    with client:
        #check correct id's
        response=client.get('/spell_check')
        #print("RESPONSE",response.data)
        token=response.data.decode().split("csrf_token")[1].split("value=")[1].split("\"/>")[0].split("TOKEN \"")[0][1:]
        print("TOKEN",token)
        assert client.get('/spell_check').status_code==200
        assert b"id=\"inputtext\"" in response.data

        
        #check if spell_check accepts POST
        
        #response submitted with csrf token
        response = client.post('/spell_check',
            data={'inputtext': 'Wrog Spellings','csrf_token':token}
        )
        assert b"id=\"textout\"" in response.data
        assert b"id=\"misspelled\"" in response.data
        
        #response submitted without csrf token
        response = client.post('/spell_check',
            data={'inputtext': 'Wrog Spellings'}
        )
        assert b"id=\"textout\"" not in response.data
        assert b"id=\"misspelled\""not in response.data
        
        

#checking various cases of login with wrong credentials, wrong 2fa code , and correct credentials
@pytest.mark.parametrize(('username', 'password', 'phone', 'message'), (
    ('reg1', 'test','1234567890', b'Incorrect credentials'),
    ('reg1', 'reg1','12367890', b'Two-factor authentication failure'),
    ('reg1','reg1', '1234567890', b'Success')
))
def test_login_validate_input(auth, username, password, phone, message):
    response = auth.login(username, password,phone)
    #print(response.data)
    assert message in response.data
    
    
    assert b"id=\"result\"" in response.data
    
    
    
