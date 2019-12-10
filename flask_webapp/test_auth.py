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
#cannot allow users to register as admin user, it should already exist in the database.
@pytest.mark.parametrize(('username', 'password', 'phone', 'message'), (
    ('reg1', 'test','1234567890', b'success'),
    ('reg1', 'tes1','12367890', b'failure'),
    ('admin', 'tes1','12367890', b'failure')
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





def test_admin_login(client,auth):
    #assert client.get('/login').status_code == 200
    response = auth.admin_login()
    #print(response.data)
    with client:
        #logged in clients must be redirected away from  login page
        assert client.get('/login').status_code==302
        assert client.get('/spell_check').status_code==200
        assert session['user_id'] == 3
        assert g.user['username'] == 'admin'

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
        
#Checks the history functionality for a normal user
def test_history_normal(client,auth):
    response = auth.login()
    
    with client:
        response=client.get('/spell_check')
        #print("RESPONSE",response.data)
        token=response.data.decode().split("csrf_token")[1].split("value=")[1].split("\"/>")[0].split("TOKEN \"")[0][1:]
        
        #submitting two responses
        response = client.post('/spell_check',
            data={'inputtext': 'Wrog Spellings','csrf_token':token}
        )
        response = client.post('/spell_check',
            data={'inputtext': 'all correct spellings here','csrf_token':token}
        )
        
        history_response = client.get('/history')
        # 2 queries should be present
        assert b"<p class=\"mb-0\" id=\"numqueries\">Total queries:" in history_response.data
        
        #Get request to only the queries submitted by user should be allowed.
        #print(type(history_response.data))
        list_items= history_response.data.decode().split("id=\"query")
        i = 1
        auth_query_ids=[]
        while i < len(list_items):
            auth_query_ids.append(int(list_items[i].split("\"")[0]))
            i+=1
        response=client.get('/spell_check')
        #Since non-admin no userid element should be presented for inputtext
        assert b"id=\"userquery\"" not in history_response.data
        
        assert len(auth_query_ids)==2
        
        query_request_string="/history/query"
        
        for i in auth_query_ids:
            qs=query_request_string+str(i)
            query_response = client.get(qs)
            #print(query_response.data.decode())
            #Checking if user has access to his own queries
            assert "not allowed" not in query_response.data.decode()
            
            #Checking whether the id's output are accurate
            assert "id=\"username\"" in query_response.data.decode()
            assert "id=\"queryid\"" in query_response.data.decode()
            assert "id=\"querytext\"" in query_response.data.decode()
            assert "id=\"queryresults\"" in query_response.data.decode()
            
        #Try to access an unauthorized query, should be disallowed
        qs=query_request_string+str(8)
        query_response = client.get(qs)
        assert "disallowed" in query_response.data.decode()
        
        
        #non admin should not be able to post to /history
        user_response = client.post('/history',
            data={'userid': 'admin','csrf_token':token}
        )
        #since this is not an admin user, 200 should not be returned
        assert user_response.status_code!=200

def test_history_admin(client,auth):
    response = auth.admin_login()
    with client:
        response=client.get('/spell_check')
        #print("RESPONSE",response.data)
        token=response.data.decode().split("csrf_token")[1].split("value=")[1].split("\"/>")[0].split("TOKEN \"")[0][1:]
        history_response = client.get('/history')
        # 2 queries should be present
        #print(history_response.data.decode())
        assert b"id=\"userquery\"" in history_response.data
        user_response = client.post('/history',
            data={'userid': 'reg1','csrf_token':token}
        )
        print(user_response.data.decode())
        print(user_response.status_code)
        
        #allow admin user to post a username
        assert user_response.status_code==200
        assert "<h2>Queries Made by  reg1 </h2>" in user_response.data.decode()

#non-admin user should not be able to see logs of any kind
def test_user_logs_normal(client,auth):
    response = auth.login()
    with client:
        user_response = client.get('/login_history')
        user_response_post = client.post('/login_history',
            data={'userid':'admin'}
        )
        assert user_response.status_code != 200
        assert user_response_post.status !=200

def test_user_logs_admin(client,auth):
    response = auth.admin_login()
    with client:
        user_response = client.get('/login_history')
        assert user_response.status_code == 200
        token=user_response.data.decode().split("csrf_token")[1].split("value=")[1].split("\"/>")[0].split("TOKEN \"")[0][1:]
        
        user_response_post = client.post('/login_history',
            data={'userid':'reg1','csrf_token':token}
        )
        
        assert user_response_post.status_code ==200
        assert "<h2>User Activity for reg1</h2>" in user_response_post.data.decode()



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
    
    
    
