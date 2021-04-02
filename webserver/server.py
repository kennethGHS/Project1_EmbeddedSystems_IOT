from http.server import BaseHTTPRequestHandler, HTTPServer

host_port = 8000


class Server(BaseHTTPRequestHandler):

    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
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
        self.send_response(200)
        self.do_HEAD()
        self.end_headers()

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
        picture = open(path, mode="wb")
        picture.write(data)
        picture.close()
        self.do_HEAD()

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
