from pubnub.callbacks import SubscribeCallback
from pubnub.enums import PNStatusCategory
from pubnub.pnconfiguration import PNConfiguration
from pubnub.pubnub import PubNub

ENTRY = "Alert"
CHANNEL = "detection-channel"
the_update = None

pnconfig = PNConfiguration()
pnconfig.publish_key = "pub-c-82a0abf0-186c-47ae-8db9-ef683eb3a68c"
pnconfig.subscribe_key = "sub-c-bd5ecdfe-113b-11ec-b08e-9adb7f1f2877"
pnconfig.uuid = "serverUUID-PUB"

pubnub = PubNub(pnconfig)


def publish_alert(data):
    envelope = pubnub.publish().channel(CHANNEL).message(data).sync()

    if envelope.status.is_error():
        print("[PUBLISH: fail]")
        print("error: %s" % envelope.status.error)
    else:
        print("[PUBLISH: sent]")
        print("timetoken: %s" % envelope.result.timetoken)
