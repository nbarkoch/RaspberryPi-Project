from dataclasses import dataclass
from datetime import datetime


@dataclass
class Station:
    id: str
    name: str
    city: str


@dataclass
class Camera:
    id: str
    name: str
    stationID: int
    active: bool


@dataclass
class Alert:
    id: str
    cameraID: str
    imageURL: str
    videoURL: str
    registeredDate: datetime.time # date and time format
    seen: bool  # (false if new or not seen/ true if old)

