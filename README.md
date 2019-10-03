# Project 03: Message Queue

This is [Project 03] of [CSE.30341.FA19].

## Students

1. Rob Reutiman (rreutima@nd.edu)

## Brainstorming

The following are questions that should help you in thinking about how to
approach implementing [Project 03].  For this project, responses to these
brainstorming questions are not required.

### Request

1. What data must be allocated and deallocated for each `Request` structure?

2. What does a valid **HTTP** request look like?

### Queue

1. What data must be allocated and deallocated for each `Queue` structure?

2. How will you implement **mutal exclusion**?

3. How will you implement **signaling**?

3. What are the the **critical sections**?

### Client

1. What data must be allocated and deallocated for each `MessageQueue`
   structure?

2. What should happen when the user **publishes** a message?

3. What should happen when the user **retrieves** a message?

4. What should happen when the user **subscribes** to a topic?

5. What should happen when the user **unsubscribes** to a topic?

6. What needs to happen when the user **starts** the `MessageQueue`?

7. What needs to happen when the user **shuts down** the `MessageQueue`?

8. How many internal **threads** are required?

9. What is the purpose of each internal **thread**?

10. What `MessageQueue` attribute needs to be **protected** from **concurrent**
    access?

## Demonstration

> Link to Video demonstration of user application.

## Errata

> Describe any known errors, bugs, or deviations from the requirements.

## Extra Credit

> Describe what extra credit (if any) that you implemented.

[Project 03]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa19/project03.html
[CSE.30341.FA19]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa19/
