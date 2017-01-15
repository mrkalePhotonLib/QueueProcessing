/*
  NAME:
  QueueProcessing

  DESCRIPTION:
  The library provides standard C++ queueing for storing event messages and
  finally publishes them to the Particle Cloud. Each event is defined
  by its name, text, and scope (private or public).

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
  (related to original code) and MIT License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkalePhotonLib/QueueProcessing.git
  
  CREDITS:
  - Tiago Andrade e Silva: Library PublishQueue
    (https://github.com/tiagonmas/publishqueue-library-spark.git)
 */
#ifndef QUEUEPROCESSING_H
#define QUEUEPROCESSING_H

#define QUEUEPROCESSING_VERSION "QueueProcessing 1.0.0"

#include "Particle.h"
#include <queue>

#define QUEUEPROCESSING_PUBLISH_PUBLIC  0       // The value determining public event
#define QUEUEPROCESSING_PUBLISH_PRIVATE 1       // The value determining private event

#define QUEUEPROCESSING_PUBLISH_PERIOD_DEF 1000 // The default publishing delay in milliseconds
#define QUEUEPROCESSING_PUBLISH_PERIOD_MIN 1000 // The minimal publishing delay in milliseconds

#define QUEUEPROCESSING_PUBLISH_COUNT_DEF 4     // The default number of event in the publishing batch
#define QUEUEPROCESSING_PUBLISH_COUNT_MIN 1     // The minimal number of event in the publishing batch
#define QUEUEPROCESSING_PUBLISH_COUNT_MAX 4     // The maximal number of event in the publishing batch

struct QueueProcessingEvent {
    String  name;
    String  text;
    uint8_t scope;
};

class QueueProcessing
{
public:
//-------------------------------------------------------------------------
// Public methods
//-------------------------------------------------------------------------

/*
  Constructor

  DESCRIPTION:
    Constructor creates a class instance object. It initially uses all
    default internal processing parameters of the library. Some of them can
    be altered by corresponding setters.

  PARAMETERS: none

  RETURN: object
*/
  QueueProcessing();


/*
  Put an event to the queue

  DESCRIPTION:
  The method puts the event with input parameters to the queue. The method
  is overloaded and can receive either separate event parameters or a structure
  with them.

  PARAMETERS:
  eventName - The name of an event in the queue.
                - Data type: String
                - Default value: none
                - Limited range: 1 ~ 63 ASCII characters

  eventText - The content of an event to be put to the queue.
                - Data type: String
                - Default value: none
                - Limited range: 0 ~ 255 bytes

  eventScope    - The flag determining the public or private event.
                - Data type: non-negative integer
                - Default value: QUEUEPROCESSING_PUBLISH_PRIVATE
                - Limited range: QUEUEPROCESSING_PUBLISH_PRIVATE
                              or QUEUEPROCESSING_PUBLISH_PUBLIC

  event     - The event in a form of structure with event parameters.
                - Data type: QueueProcessingEvent
                - Default value: none
                - Limited range: 1 object

  RETURN: none
*/
  void enqueue(String eventName, String eventText, uint8_t eventScope = QUEUEPROCESSING_PUBLISH_PRIVATE);
  void enqueue(QueueProcessingEvent event);


/*
  Get the top event and remove it from the queue

  DESCRIPTION:
  The method returns the top (front) event and removes it from the queue.
  - This method is intended for one time usage of queued event.
  
  PARAMETERS: none

  RETURN:
  event - The event in a form of structure with event parameters.
*/
  QueueProcessingEvent dequeue();


/*
  Get the top event from the queue without removing it

  DESCRIPTION:
  The method returns the top (front) event but leaves it in the queue intact.

  PARAMETERS: none

  RETURN:
  event - The event in a form of structure with event parameters.
*/
  QueueProcessingEvent peek();


/*
  Get the name of a top event in the queue

  DESCRIPTION:
  The method returns the name of top (front) event but leaves the event
  in the queue intact.

  PARAMETERS: none

  RETURN:
  The pointer to the name of an event
*/
  String peekName();


/*
  Get the text of a top event in the queue

  DESCRIPTION:
  The method returns the text of top (front) event but leaves the event
  in the queue intact.

  PARAMETERS: none

  RETURN:
  The content of an event
*/
  String peekText();


/*
  Get the scope of a top event in the queue

  DESCRIPTION:
  The method returns the flag about scope of top (front) event but leaves
  the event in the queue intact.

  PARAMETERS: none

  RETURN:
  The scope flag of an event, either QUEUEPROCESSING_PUBLISH_PUBLIC or 
  QUEUEPROCESSING_PUBLISH_PRIVATE.
*/
  uint8_t peekScope();


/*
  Clear the queue by removing all its events

  DESCRIPTION:
  The method removes all event currently stored in the queue and makes it empty.

  PARAMETERS: none

  RETURN: none
*/
  void clear();


/*
  Publish the top event in the queue

  DESCRIPTION:
  The method publishes one event at the front of the queue to the Particle
  Cloud with respect to defined publishing period.
  - The method is intended to run in a loop of an application and publish
    queued events one by one evenly.
  - If the publishing is unsuccessful, e.g., due to disconneting from the
    Particle Cloud, the method leaves particular event in the queue and
    tries to publish it in the next run. Otherwise at successful publishing
    the method removes that event from the queue.
  
  PARAMETERS: none

  RETURN:
  The success flag
*/
  bool publish();


/*
  Publish the batch of top events in the queue

  DESCRIPTION:
  The method publishes maximal batch number of front events in the queue
  to the Particle Cloud with respect to defined publishing period.
  This method is intended to run in a timer and publish queued events
  in bursts.

  PARAMETERS: none

  RETURN:
  The success flag
*/
  bool publishBatch();


//-------------------------------------------------------------------------
// Public setters
//-------------------------------------------------------------------------

/*
  Set publishing period in milliseconds

  DESCRIPTION:
  The method defines the publishing period for publishing individual events.
  - The method overrides the default value.

  PARAMETERS:
  period - The publishing period in milliseconds.
           - Data type: non-negative integer
           - Default value: 1000 (QUEUEPROCESSING_PUBLISH_PERIOD_DEF)
           - Limited range: 0 ~ 65535

  RETURN: none
*/
  void setPublishPeriod(uint16_t period = QUEUEPROCESSING_PUBLISH_PERIOD_DEF);


/*
  Set number of events in a publishing batch

  DESCRIPTION:
  The method defines the count of events that should be published at once
  in an publishing burst.

  PARAMETERS:
  number - The number of publishing events in a batch.
           - Data type: non-negative integer
           - Default value: 4 (QUEUEPROCESSING_PUBLISH_COUNT_DEF)
           - Limited range: 1 ~ 4 (QUEUEPROCESSING_PUBLISH_COUNT_MIN ~ QUEUEPROCESSING_PUBLISH_COUNT_MAX)

  RETURN: none
*/
  void setPublishBatch(uint8_t number = QUEUEPROCESSING_PUBLISH_COUNT_DEF);


//-------------------------------------------------------------------------
// Public getters
//-------------------------------------------------------------------------

/*
  Get flag whether the queue is empty

  DESCRIPTION:
  The method returns the flag whether the queue is empty.

  PARAMETERS: none

  RETURN:
  The flag whether the queue is empty
*/
  bool isEmpty();


  /*
  Get current number of events in the queue.

  DESCRIPTION:
  The method returns the length of the queue counting in number of events
  currently present in the queue.

  PARAMETERS: none

  RETURN:
  The number of events in the queue
*/
  uint16_t getQueueLen();


/*
  Getters for current values of parameters defined by corresponding setters
  in advance.
*/
  uint16_t getPublishPeriod();
  uint8_t  getPublishBatch();


private:
//-------------------------------------------------------------------------
// Private attributes
//-------------------------------------------------------------------------
  std::queue<QueueProcessingEvent> _queue; // Event queue
  uint16_t _publishPeriod = QUEUEPROCESSING_PUBLISH_PERIOD_DEF; // Publishing period in milliseconds
  uint8_t  _publishBatch = QUEUEPROCESSING_PUBLISH_COUNT_DEF;   // Publishing batch number
  uint32_t _tsPublish;          // Timestamp of the recent publishing

//-------------------------------------------------------------------------
// Private methods
//-------------------------------------------------------------------------
};

#endif
