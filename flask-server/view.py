import json

from flask import Flask
from datetime import datetime
from dbTrainStations import StationWrapper, CameraWrapper, AlertWrapper, Controller
from pub import publish_alert
app = Flask(__name__)

REAL_TIME = 1
S3_UPDATE = 2



# Controller
@app.route('/')
def hello_world():
    return 'Hello, World! trainServer!'


@app.route('/get_all_stations_details')
def get_all_stations_details():
    return json.dumps(Controller.get_all_stations_details())


# StationWrapper
@app.route('/StationWrapper/get_by_id/<station_id>')
def station_get_by_id(station_id):
    return json.dumps(StationWrapper.get_by_id(station_id))


@app.route('/StationWrapper/get_by_city_and_name/<station_name>/<station_city>')
def station_get_by_city_and_name(station_name, station_city):
    return str(StationWrapper.get_by_city_and_name(station_name, station_city))


@app.route('/StationWrapper/get_all_stations')
def station_get_all_stations():
    return json.dumps(StationWrapper.get_all_stations())


@app.route('/StationWrapper/create/<station_name>/<station_city>')
def station_create(station_name, station_city):
    return json.dumps(StationWrapper.create(station_name, station_city))


# CameraWrapper:
@app.route('/CameraWrapper/get_by_id/<camera_id>')
def camera_get_by_id(camera_id):
    return json.dumps(CameraWrapper.get_by_id(camera_id))


@app.route('/CameraWrapper/get_all_cameras')
def camera_get_all_cameras():
    return json.dumps(CameraWrapper.get_all_cameras())


@app.route('/CameraWrapper/create/<camera_name>/<stationID>/<active>')
def camera_create(camera_name, stationID, active):
    if active == 1 or active == '1':
        act = True
    else:
        act = False
    return json.dumps(CameraWrapper.create(camera_name, stationID, act))


# AlertWrapper:
@app.route('/AlertWrapper/get_by_id/<alert_id>')
def alert_get_by_id(alert_id):
    return json.dumps(AlertWrapper.get_by_id(alert_id))


@app.route('/AlertWrapper/get_all_alerts')
def alert_get_all_alerts():
    return json.dumps(AlertWrapper.get_all_alerts())


# try http://localhost:9003/AlertWrapper/create/9e5c0e0f-40c9-48e3-bad6-51c6464dfac1/matan/omesi/false
@app.route('/AlertWrapper/create/<cameraID>/<imageURL>/<videoURL>/<seen>')
def alert_camera_create(cameraID, imageURL, videoURL, seen):
    if seen == 1 or seen == '1':
        is_seen = True
    else:
        is_seen = False
    camera_data = CameraWrapper.get_by_id(cameraID)
    if camera_data:
        alert_data = AlertWrapper.create(cameraID, imageURL, videoURL, str(datetime.now()), is_seen)
        publish_alert({'stationID': camera_data['stationID'], 'cameraID': cameraID,
                       'alert': alert_data})
        return json.dumps(alert_data)
    return str(None)

# try http://localhost:9003/AlertWrapper/pubnub/d918d447-8260-47f1-ab44-5486b0985a48/f9a5eef4-ed21-4a40-8450-a16ddbc4ea6d/1631490840
@app.route('/AlertWrapper/pubnub/<stationID>/<cameraID>/<fileID>')
def alert_pubnub_realtime(stationID, cameraID, fileID):

    alert_data = AlertWrapper.create(cameraID, fileID, str(datetime.now()), False)
    publish_alert({'type': REAL_TIME, 'stationID': stationID, 'cameraID': cameraID,
                   'alert': alert_data})
    return json.dumps(alert_data)

# try http://localhost:9003/S3Wrapper/pubnub/d918d447-8260-47f1-ab44-5486b0985a48/f9a5eef4-ed21-4a40-8450-a16ddbc4ea6d/1631490840
@app.route('/S3Wrapper/pubnub/<stationID>/<cameraID>/<fileID>')
def alert_pubnub_s3upload(stationID, cameraID, fileID):

    AlertWrapper.update_s3_upload(cameraID, fileID)
    publish_alert({'type': S3_UPDATE,'stationID': stationID, 'cameraID': cameraID, 'fileID': fileID})
    return str(None)



