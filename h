from bottle import route, run, request, response, redirect, static_file

user_database = {
    'user1': 'password1',
    'user2': 'password2',
}

def authenticate(username, password):
    if username in user_database and user_database[username] == password:
        return True
    return False

def set_authenticated_cookie():
    response.set_cookie('authenticated', 'true', path='/', max_age=300)  # 5 minutes

def authenticated():
    return request.get_cookie('authenticated') == 'true'

@route('/')
def index():
    return static_file('index.html', root='./views')

@route('/login')
def login():
    return static_file('login.html', root='./views')

@route('/authenticate', method='POST')
def do_login():
    username = request.forms.get('username')
    password = request.forms.get('password')
    if authenticate(username, password):
        set_authenticated_cookie()
        return redirect('/')
    else:
        return redirect('/unauthorized')

@route('/secured')
def secured_page():
    if not authenticated():
        return redirect('/unauthorized')
    return static_file('secured_page.html', root='./views')

@route('/unauthorized')
def unauthorized():
    return static_file('unauthorized.html', root='./views')

@route('/logout')
def logout():
    response.delete_cookie('authenticated')
    return redirect('/')

@route('/static/<filename:path>')
def serve_static(filename):
    return static_file(filename, root='./static')

# Use route to attach routes directly without declaring an app variable
if __name__ == '__main__':
    run(host='localhost', port=8080)
