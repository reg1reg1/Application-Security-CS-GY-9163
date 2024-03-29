import tempfile
import os
import pytest
from app import create_app
from db import get_db, init_db
from werkzeug.security import generate_password_hash
with open(os.path.join(os.path.dirname(__file__), 'test.sql'), 'rb') as f:
    _data_sql = f.read().decode('utf8')


@pytest.fixture
def app():
    db_fd, db_path = tempfile.mkstemp()

    app = create_app({
        'TESTING': True,
        'DATABASE': db_path,
    })

    with app.app_context():
        init_db()
        get_db().executescript(_data_sql)
        dbobj = get_db()
        #Adding the admin user for test cases.
        dbobj.execute('INSERT INTO user (username, password,phone, isAdmin) VALUES (?, ?, ?, ?)',('admin', generate_password_hash('Administrator@1'),'12345678901',True))
        dbobj.commit()

    yield app

    os.close(db_fd)
    os.unlink(db_path)

class AuthActions(object):
    def __init__(self, client):
        self._client = client

    def login(self, username='reg1', password='reg1',twofa='1234567890'):
        return self._client.post(
            '/login',
            data={'uname': username, 'pword': password,'2fa':twofa}
            ,follow_redirects=True
        )
    
    #login defined for testing admin user functionality
    def admin_login(self, username='admin', password='Administrator@1',twofa='12345678901'):
        return self._client.post(
            '/login',
            data={'uname': username, 'pword': password,'2fa':twofa}
            ,follow_redirects=True
        )

    def logout(self):
        return self._client.get('/logout')


@pytest.fixture
def auth(client):
    return AuthActions(client)



@pytest.fixture
def client(app):
    return app.test_client()


@pytest.fixture
def runner(app):
    return app.test_cli_runner()
