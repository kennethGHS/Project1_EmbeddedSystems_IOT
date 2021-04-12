import json
import mimetypes
from http.server import BaseHTTPRequestHandler, HTTPServer
from uuid import uuid4

host_port = 8000


def save_token(token):
    """This function saves a session token to disk"""

    path = "tokens.json"
    file = open(path, mode="r+")
    tokens = json.loads(file.read())
    tokens.append(token)
    file.seek(0)
    file.truncate()
    json.dump(tokens, file)
    file.close()


class Server(BaseHTTPRequestHandler):
    '''HTTP server with a RESTful API'''

    def do_HEAD(self):
        """This method generates the header for every request path"""

        self.send_response(200)
        mimetype, _ = mimetypes.guess_type(self.path)
        self.send_header('Content-type', mimetype)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, OPTIONS')
        self.send_header("Access-Control-Allow-Headers", "X-Requested-With")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()

    def do_GET(self):
        """This method resolves GET requests to the server, whether its for the API or a website"""

        path = self.path.split("?")[0]
        if self.path == "/":
            self.send_file("website/common/login.html")
        elif self.path == "/api/sensors":
            self.send_file("sensors.json")
        elif self.path == "/api/lights":
            self.send_file("lights.json")
        elif self.path == "/api/take_picture":
            self.send_file("take_picture.json")
        elif path == "/api/picture":
            self.send_file("picture.jpeg")
        else:
            self.send_file("website" + self.path)

    def do_POST(self):
        """This method resolves POST requests to the API"""

        # Get the size of data
        content_length = int(self.headers['Content-Length'])
        # Get the data
        post_data = self.rfile.read(content_length)

        if self.path == "/api/sensors":
            self.receive_file("sensors.json", post_data)
        elif self.path == "/api/lights":
            self.receive_file("lights.json", post_data)
        elif self.path == "/api/take_picture":
            self.receive_file("take_picture.json", post_data)
        elif self.path == "/api/auth":
            self.authenticate("user_db.json", post_data)
        elif self.path == "/api/sign":
            self.sign_up("user_db.json", post_data)
        elif self.path == "/api/session":
            self.validate_session("tokens.json", post_data)
        else:
            self.resource_not_found()

    def do_PUT(self):
        """This method resolves PUT requests to the API"""

        # Get the size of data
        content_length = int(self.headers['Content-Length'])
        # Get the data
        put_data = self.rfile.read(content_length)

        if self.path == "/api/picture":
            self.receive_file("picture.jpeg", put_data)
            file = open("take_picture.json", mode="w")
            file.write('{"photo": 0}')
            file.close()
        else:
            self.resource_not_found()

    def do_OPTIONS(self):
        """This method resolves OPTIONS requests to any path"""
        self.do_HEAD()

    def send_file(self, path):
        """This method reads a file from disk and sends it as response of a request"""

        try:
            file = open(path, mode='rb')
            data = file.read()
            file.close()
            self.do_HEAD()
            self.wfile.write(data)
        except IOError:
            self.resource_not_found()

    def receive_file(self, path, data):
        """This method write data received as a request to a file on disk"""

        file = open(path, mode="wb")
        file.write(data)
        file.close()
        self.do_HEAD()

    def authenticate(self, path, data):
        """This method authenticates a user given the username and password"""

        file = open(path, mode='r+')
        users = json.loads(file.read())
        data = json.loads(data)
        for user in users:
            if user["username"] == data["username"] and user["password"] == data["password"]:
                token = str(uuid4())
                save_token(token)
                self.send_token(token)
                return
        token = str(0)
        self.send_token(token)

    def sign_up(self, path, data):
        """This method registers a new user and grants permissions to use the platform"""

        file = open(path, mode='r+')
        users = json.loads(file.read())
        data = json.loads(data)
        users.append(data)
        file.seek(0)
        file.truncate()
        json.dump(users, file)
        file.close()

    def validate_session(self, path, data):
        """This method validates the current user session given the session token"""

        file = open(path, mode='r+')
        tokens = json.loads(file.read())
        data = json.loads(data)
        value = "invalid"
        for token in tokens:
            if token == data["token"]:
                value = "valid"
                break
        self.do_HEAD()
        self.wfile.write(json.dumps({"token": value}).encode())
        file.close()

    def send_token(self, token):
        """This method sends the session token as part of the HTTP header"""

        self.send_response(200)
        self.send_header("Authorization", token)
        self.end_headers()

    def resource_not_found(self):
        """This method responds with HTTP error 404 when a resource is not found"""

        self.send_response(404)
        self.send_header('Content-type', 'text/html')
        self.end_headers()


if __name__ == '__main__':
    http_server = HTTPServer(('', host_port), Server)
    print("Server started on port: %s" % host_port)

    try:
        http_server.serve_forever()
    except KeyboardInterrupt:
        http_server.server_close()
