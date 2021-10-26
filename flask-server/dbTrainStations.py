from tinydb.table import Document

from primitives import Station, Camera, Alert
from tinydb import TinyDB, Query
import uuid

DB_PATH = 'stations-db.json'
db = TinyDB(DB_PATH)
stations_table = db.table('stations')
cameras_table = db.table('cameras')
alerts_table = db.table('alerts')


class Controller:

    @staticmethod
    def get_all_stations_details():
        all_details = []
        stations = stations_table.all()
        for i_station in stations:
            # station_details = {}
            cameras = CameraWrapper.get_cameras_by_station(i_station["id"])
            for j, j_camera in enumerate(cameras):
                alerts = AlertWrapper.get_alerts_by_camera(j_camera["id"])
                cameras[j]['alerts'] = alerts
                # station_details[str(j_camera)] = alerts
            i_station['cameras'] = cameras
            all_details.append(i_station)
            # print(type(i_station))
            # all_details[str(i_station)] = station_details
        return all_details


class StationWrapper:

    @classmethod
    def get_by_id(cls, station_id: str) -> Station:
        return stations_table.search(Query().id == station_id)[0]

    @classmethod
    def get_by_city_and_name(cls, station_name: str, station_city: str):
        QStation = Query()
        return stations_table.search((QStation.name == station_name) & (QStation.city == station_city))

    @classmethod
    def get_all_stations(cls):
        return stations_table.all()

    @classmethod
    def create(cls, station_name: str, station_city: str):
        if cls.get_by_city_and_name(station_name, station_city):
            raise Exception('Station already exists')
        station_data = {"id": str(uuid.uuid4()), "name": station_name, "city": station_city}
        stations_table.insert(station_data)
        return station_data


class CameraWrapper:

    @classmethod
    def get_by_id(cls, camera_id: str):
        return cameras_table.search(Query().id == camera_id)[0]

    @classmethod
    def get_cameras_by_station(cls, stationID: str):
        return cameras_table.search(Query().stationID == stationID)

    @classmethod
    def get_all_cameras(cls):
        return cameras_table.all()

    @classmethod
    def create(cls, camera_name: str, stationID: str, active: bool):
        camera_data = {"id": str(uuid.uuid4()), "name": camera_name, "stationID": stationID, "active": active}
        cameras_table.insert(camera_data)
        return camera_data


class AlertWrapper:

    @classmethod
    def get_by_id(cls, alert_id: str) -> Document:
        return alerts_table.search(Query().id == alert_id)[0]

    @classmethod
    def get_alerts_by_camera(cls, cameraID: str):
        return alerts_table.search(Query().cameraID == cameraID)

    @classmethod
    def update_s3_upload(cls, camera_id: str, file_id: str) -> None:
        # alerts_table.upsert({'s3Upload': True}, (Query().fileID == file_id and Query().cameraID == camera_id))
        return None

    @classmethod
    def create(cls, cameraID: str, fileID: str, registeredDate: str, seen: bool) -> dict:
        alert_data = {"id": str(uuid.uuid4()), "cameraID": cameraID, "fileID": fileID, "s3Upload": False,
                             "registeredDate": registeredDate, "seen": seen}
        # alerts_table.insert(alert_data) # don't forget to uncomment this for allowing saving alert data
        return alert_data

    @classmethod
    def get_all_alerts(cls):
        return alerts_table.all()

