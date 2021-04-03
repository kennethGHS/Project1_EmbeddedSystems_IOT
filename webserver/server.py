import json
import mimetypes
from http.server import BaseHTTPRequestHandler, HTTPServer
from uuid import uuid4

host_port = 8000


def save_token(token):
    path = "tokens.json"
    file = open(path, mode="r+")
    tokens = json.loads(file.read())
    tokens.append(token)
    file.seek(0)
    file.truncate()
    json.dump(tokens, file)
    file.close()


class Server(BaseHTTPRequestHandler):

    def do_HEAD(self):
        self.send_response(200)
        mimetype, _ = mimetypes.guess_type(self.path)
        self.send_header('Content-type', mimetype)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, OPTIONS')
        self.send_header("Access-Control-Allow-Headers", "X-Requested-With")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()

    def do_GET(self):
        if self.path == "/":
            self.send_file("website/index.html")
        elif self.path == "/api/sensors":
            self.send_file("sensors.json")
        elif self.path == "/api/lights":
            self.send_file("lights.json")
        elif self.path == "/api/take_picture":
            self.send_file("take_picture.json")
            file = open("take_picture.json", mode="w")
            file.write('{"photo":"0"}')
            file.close()
        elif self.path == "/api/picture":
            self.send_file("picture.jpg")
        else:
            self.send_file("website" + self.path)

    def do_POST(self):
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
        # Get the size of data
        content_length = int(self.headers['Content-Length'])
        # Get the data
        put_data = self.rfile.read(content_length)

        if self.path == "/api/picture":
            self.receive_file("picture.jpg", put_data)
        else:
            self.resource_not_found()

    def do_OPTIONS(self):
        self.do_HEAD()

    def send_file(self, path):
        try:
            file = open(path, mode='rb')
            data = file.read()
            file.close()
            self.do_HEAD()
            self.wfile.write(data)
        except IOError:
            self.resource_not_found()

    def receive_file(self, path, data):
        file = open(path, mode="wb")
        file.write(data)
        file.close()
        self.do_HEAD()

    def authenticate(self, path, data):
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
        file = open(path, mode='r+')
        users = json.loads(file.read())
        data = json.loads(data)
        users.append(data)
        file.seek(0)
        file.truncate()
        json.dump(users, file)
        file.close()

    def validate_session(self, path, data):
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
        self.send_response(200)
        self.send_header("Authorization", token)
        self.end_headers()

    def resource_not_found(self):
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