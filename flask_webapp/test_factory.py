from app import create_app


def test_config():
    assert not create_app().testing
    assert create_app({'TESTING': True}).testing

#basic routes as described in the app description
def test_get_routes(client):
    response = client.get('/')
    assert response.status_code == 200
    
    
    response = client.get('/login')
    assert b"name=\"uname\" id=\"uname\"" in response.data
    assert b"name=\"pword\" id=\"pword\"" in response.data
    assert b"name=\"2fa\" id=\"2fa\"" in response.data
    assert response.status_code == 200
    
    
    
    response = client.get('/register')
    assert b"name=\"uname\" id=\"uname\"" in response.data
    assert b"name=\"pword\" id=\"pword\"" in response.data
    assert b"name=\"2fa\" id=\"2fa\"" in response.data
    assert response.status_code == 200
    
    
    #Unauthenticated users must be redirected to login page
    response =  client.get('/spell_check')
    assert response.status_code == 302
    
    

    
