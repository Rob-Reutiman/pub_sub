# OS Project 03

For this project, I created a basic chat application that entails a message queue client interacting with a rudimentary pub/sub system using POSIX threads and network sockets via a RESTful API. Clients in the pub/sub system connect to this server and perform the following operations:

(1) SUBSCRIBE: This associates a queue to a particular topic.

Subscribing to a topic means that any messages sent to the {TOPIC} topic will be automatically forwarded to the {SUBSCIRBER} queue. Clients can subscribe to as many topics as they wish. However, they will only receive messages after they have subscribed (any messages sent to the topic before they subscribe) will not be accessible.

(2) PUBLISH: This posts a message to a particular topic.

The client sends a HTTP PUT command to publish a message to the {TOPIC} topic with a message body: {MESSAGE}. Internally, the pub/sub server will see that {SUBSCRIBER} is subscribed to the the {TOPIC} topic, and thus it will forward the message to the {SUBSCRIBER} queue.

(3) RETRIEVE: This fetches one message in the queue.

The client sends a HTTP GET command to retrieve a message from the {SUBSCRIBER} queue. Internally, the pub/sub server will fetch one message from the {SUBSCRIBER} queue and return it as the response to the HTTP request. When clients retrieve a message but the corresponding queue is empty, then the pub/sub server will delay responding to the request until there is something in the queue. This means that performing a retrieve operation is a blocking action for the client.
