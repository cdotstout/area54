from flask import Flask, render_template, request
from flask_redis import FlaskRedis


def get_app():
    app = Flask(__name__)
    app.config.from_object(__name__)
    app.config.update({
        'SECRET_KEY': '36439a24-a27d-4ee5-8b6c-ed1a357ffde9',
        'REDIS_URL': 'redis://localhost:6379/0',
    })
    return app


def get_redis(app):
    return FlaskRedis(app)


app = get_app()
redis_store = get_redis(app)


@app.route('/', methods=('GET', 'POST'))
def index():
    if request.method == 'GET':
        bpm = redis_store.get('bpm') or 120
        return render_template('bpm.html', bpm=int(bpm))

    else:
        bpm = int(request.form['bpm'])
        redis_store.set('bpm', bpm)
        return 'OK'


if __name__ == '__main__':
    app.run('0.0.0.0', port=8000, debug=False)

