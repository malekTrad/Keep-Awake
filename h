from bottle import Bottle, request, response, route, run, redirect
from bottle_mako import MakoTemplatePlugin, view
import session

app = Bottle()
session_plugin = session.SessionPlugin(cookie_lifetime=600)
app.install(session_plugin)

mako_plugin = MakoTemplatePlugin(
    directories=['views'],  # Your Mako template directory
    default_filters=['decode.utf8'],
)
app.install(mako_plugin)

# Simulated user database (you'd typically use a database in practice)
user_database = {
    'user1': 'password1',
    'user2': 'password2',
}

def authenticate(username, password):
    if username in user_database and user_database[username] == password:
        return True
    return False

@route('/')
@view('index')  # Your Mako template name
def index():
    return {}

@route('/login')
@view('login')  # Your Mako template for the login page
def login():
    return {}

@route('/authenticate', method='POST')
def do_login():
    username = request.forms.get('username')
    password = request.forms.get('password')
    if authenticate(username, password):
        session = request.environ.get('beaker.session')
        session['authenticated'] = True
        return redirect('/')
    else:
        return "Authentication failed."

def authenticated_route(route_function):
    def wrapper(*args, **kwargs):
        session = request.environ.get('beaker.session')
        if not session.get('authenticated'):
            return redirect('/login')
        return route_function(*args, **kwargs)
    return wrapper

@route('/secured')
@authenticated_route
@view('secured_page')  # Your Mako template for secured pages
def secured_page():
    return {}

if __name__ == '__main__':
    run(app, host='localhost', port=8080)
