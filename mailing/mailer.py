import sys
import threading
import test

# Import Adafruit IO MQTT client.
from Adafruit_IO import MQTTClient

# Set to your Adafruit IO key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_KEY = '24f139eecdbb448fafa8f6cd49551a53'

# Set to your Adafruit IO username.
# (go to https://accounts.adafruit.com to find your username)
ADAFRUIT_IO_USERNAME = 'Cham3l30n'

# Set to the ID of the feed to subscribe to for updates.
FEED_ID1 = 'Temperature'
FEED_ID2 = 'Threshold'
temp=-1
thres=0


# Define callback functions which will be called when certain events happen.
def connected(client):
    # Connected function will be called when the client is connected to Adafruit IO.
    # This is a good place to subscribe to feed changes.  The client parameter
    # passed to this function is the Adafruit IO MQTT client so you can make
    # calls against it easily.
    print('Connected to Adafruit IO!  Listening for {0} changes...'.format(FEED_ID1))
    # Subscribe to changes on a feed named DemoFeed.
    client.subscribe(FEED_ID1)

def disconnected(client):
    # Disconnected function will be called when the client disconnects.
    print('Disconnected from Adafruit IO!')
    sys.exit(1)

def message(client, feed_id, payload):
    global temp
    # Message function will be called when a subscribed feed has a new value.
    # The feed_id parameter identifies the feed, and the payload parameter has
    # the new value.

    temp=float(payload)
    print('Feed {0} received new value: {1}'.format(feed_id, temp))
    print('Feed {0} received new value: {1}'.format("threshold", thres))

def message2(client, feed_id, payload):
    global thres
    # Message function will be called when a subscribed feed has a new value.
    # The feed_id parameter identifies the feed, and the payload parameter has
    # the new value.
    thres=int(payload)
    print('Feed {0} received new value: {1}'.format(feed_id, thres))
    print('Feed {0} received new value: {1}'.format("Temperature", temp))

def connected2(client):
    # Connected function will be called when the client is connected to Adafruit IO.
    # This is a good place to subscribe to feed changes.  The client parameter
    # passed to this function is the Adafruit IO MQTT client so you can make
    # calls against it easily.
    print('Connected to Adafruit IO!  Listening for {0} changes...'.format(FEED_ID2))
    # Subscribe to changes on a feed named DemoFeed.
    client.subscribe(FEED_ID2)

def task1():
    # Create an MQTT client instance.
    client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

    # Setup the callback functions defined above.
    client.on_connect = connected
    client.on_disconnect = disconnected
    client.on_message = message

    # Connect to the Adafruit IO server.
    client.connect()

    # Start a message loop that blocks forever waiting for MQTT messages to be
    # received.  Note there are other options for running the event loop like doing
    # so in a background thread--see the mqtt_client.py example to learn more.
    client.loop_blocking()

def task2():
    # Create an MQTT client instance.
    client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

    # Setup the callback functions defined above.
    client.on_connect = connected2
    client.on_disconnect = disconnected
    client.on_message = message2

    # Connect to the Adafruit IO server.
    client.connect()

    # Start a message loop that blocks forever waiting for MQTT messages to be
    # received.  Note there are other options for running the event loop like doing
    # so in a background thread--see the mqtt_client.py example to learn more.
    client.loop_blocking()

def sendmail():
    print("sending mail....")
    print("temp = "+str(temp))
    test.sendmail(temp)

def task3():
    global temp
    global thres
    flag=True
    while 1:
        if temp>=thres and flag:
            sendmail()
            flag=False
        if temp<thres:
            flag=True



t1 = threading.Thread(target=task1, name='t1')
t2 = threading.Thread(target=task2, name='t2')
t3 = threading.Thread(target=task3, name='t3')

# starting threads
t1.start()
t2.start()
t3.start()

# wait until all threads finish
# t1.join()
# t2.join()
