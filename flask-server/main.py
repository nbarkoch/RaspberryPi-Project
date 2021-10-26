from view import app
from flask_cors import CORS

cors = CORS()
if __name__ == '__main__':
    cors.init_app(app=app, supports_credentials=True)
    app.run(host='localhost', port=9003, debug=True)
