import time

from flask import Flask
from flask_restful import Api, Resource

app = Flask(__name__)
api = Api(app)


class FastEndpoint(Resource):
    def get(self):
        return {'timestamp': time.time()}


class SlowEndpoint(Resource):
    def get(self):
        a = []
        for i in range(1_000_000):
            a.append(i)
        return {'hello': a[-1]}


api.add_resource(FastEndpoint, '/fast')
api.add_resource(SlowEndpoint, '/slow')

if __name__ == '__main__':
    app.run()
