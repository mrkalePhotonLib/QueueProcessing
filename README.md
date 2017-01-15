<a id="library"></a>
# QueueProcessing
The library provides standard C++ queueing for storing event messages and finally publishes them to the *Particle Cloud*. Each event is defined by its *name*, *text*, and *scope* (private, public).
- The library is inspired by the library *PublishQueue*, but totally rewritten and extended.


<a id="dependency"></a>
## Dependency
- **Particle.h**: Include file with alternative (C++) data type definitions.
- **queue**: Standard C++ queue API.


<a id="Constants"></a>
## Constants
- **QUEUEPROCESSING\_VERSION**: Name and semantic version of the library.

- **QUEUEPROCESSING\_PUBLISH\_PUBLIC**: Value determining public event.
- **QUEUEPROCESSING\_PUBLISH\_PRIVATE**: Value determining private event.

- **QUEUEPROCESSING\_PUBLISH\_PERIOD\_DEF**: Default publishing delay in milliseconds, usually 1000.
- **QUEUEPROCESSING\_PUBLISH\_PERIOD\_MIN**: Minimal publishing delay in milliseconds, 1000 as defined by platform.

- **QUEUEPROCESSING\_PUBLISH\_COUNT\_DEF**: Default number of event in the publishing batch, usually 4.
- **QUEUEPROCESSING\_PUBLISH\_COUNT\_MIN**: Minimal number of event in the publishing batch, usually 1.
- **QUEUEPROCESSING\_PUBLISH\_COUNT\_MAX**: Minimal number of event in the publishing batch, 4 as defined by platform.


<a id="Types"></a>
## Custom data types
#### QueueProcessingEvent
It is the data structure with data and parameters of an event. It consists of following members:
- **name**: Name of the event in data type *String*.
- **text**: Text of the event (event data) in data type *String*.
- **scope**: Flag determining the public or private event at publishing in data type *unsigned char*.

<a id="interface"></a>
## Interface
- [QueueProcessing()](#QueueProcessing)
- [enqueue()](#enqueue)
- [dequeue()](#dequeue)
- [peek()](#peek)
- [peekName()](#peekName)
- [peekText()](#peekName)
- [peekScope()](#peekName)
- [clear()](#clear)
- [publish()](#publish)
- [publishBatch()](#publishBatch)

#### Setters
- [setPublishPeriod()](#setPublishPeriod)
- [setPublishBatch()](#setPublishBatch)

#### Getters
- [isEmpty()](#isEmpty)
- [getQueueLen()](#getQueueLen)
- [getPublishPeriod()](#getPublishPeriod)
- [getPublishBatch()](#getPublishPeriod)


<a id="QueueProcessing"></a>
## QueueProcessing()
#### Description
Constructor creates a class instance object. It initially uses all default internal processing parameters of the library. Some of them can be altered by corresponding setters.

#### Syntax
    QueueProcessing();

#### Parameters
None

#### Returns
Object processing the standard C++ queue with events of type [QueueProcessingEvent](#Types).

#### See also
[setPublishPeriod()](#setPublishPeriod)

[setPublishBatch()](#setPublishBatch)

[Back to interface](#interface)


<a id="enqueue"></a>
## enqueue()
#### Description
The method puts the event with input parameters to the queue.
- The method is overloaded and can receive either separate event parameters or a structure with them.

#### Syntax
    void enqueue(String eventName, String eventText, uint8_t eventScope);
    void enqueue(QueueProcessingEvent event);

#### Parameters
<a id="prm_eventName"></a>
- **eventName**: Name of an event in the queue.
  - *Valid values*: String of ASCII characters of length 1 to 63
  - *Default value*: none


<a id="prm_eventText"></a>
- **eventText**: Content (data) of an event to be put to the queue.
  - *Valid values*: String of 0 to 255 bytes.
  - *Default value*: none


<a id="prm_eventScope"></a>
- **eventScope**: Flag determining the access scope (public, private) of an event.
  - *Valid values*:
    - [QUEUEPROCESSING\_PUBLISH\_PUBLIC](#constants) for public events
    - [QUEUEPROCESSING\_PUBLISH\_PRIVATE](#constants) for private events
  - *Default value*: [QUEUEPROCESSING\_PUBLISH\_PRIVATE](#constants)

#### Returns
None

#### Example
An event can be put to the queue with its individual parameters as a private event (with default scope) or as a public event with explicit scope parameter.

``` cpp
QueueProcessing.enqueue("RSSI", String(WiFi.RSSI()));
QueueProcessing.enqueue("RSSI", String(WiFi.RSSI()), QUEUEPROCESSING_PUBLISH_PUBLIC);
```

The event structure can be prepared in advance and all parameters put to the queue at once as a private or public event.

``` cpp
QueueProcessing event = {.name = "RSSI", .text = String(WiFi.RSSI())};
QueueProcessing.enqueue(event);
QueueProcessing event = {.name = "RSSI", .text = String(WiFi.RSSI()), .scope = QUEUEPROCESSING_PUBLISH_PUBLIC};
QueueProcessing.enqueue(event);
```

#### See also
[dequeue()](#dequeue)

[Back to interface](#interface)


<a id="dequeue"></a>
## dequeue()
#### Description
The method returns the top (front) event and removes it from the queue.
- This method is intended for one time usage of a queued event.

#### Syntax
    QueueProcessingEvent dequeue();

#### Parameters
None

#### Returns
Top event in the form of data strucuture in the data type [QueueProcessingEvent](#Types).

#### See also
[enqueue()](#enqueue)

[Back to interface](#interface)


<a id="peek"></a>
## peek()
#### Description
The method returns the top (front) event but leaves it in the queue intact.
- This method is intended for multiple usage of a queued event or just reading it.

#### Syntax
    QueueProcessingEvent peek();

#### Parameters
None

#### Returns
Top event in the form of data strucuture in the data type [QueueProcessingEvent](#Types).

#### See also
[peekName()](#peekName)

[peekText()](#peekName)

[peekScope()](#peekName)

[Back to interface](#interface)


<a id="peekName"></a>
## peekName(), peekText(), peekScope()
#### Description
Each method returns the particular parameter of a top (front) event with
leaving it in the queue intact.
- Particular method is intended for multiple reading the corresponding parameter of a top event in the queue.

#### Syntax
    String peekName();
    String peekText();
    uint8_t peekScope();

#### Parameters
None

#### Returns
Particular parameter of a top event in corresponding member data type of [QueueProcessingEvent](#Types) structure.

#### See also
[peek()](#peek)

[Back to interface](#interface)


<a id="clear"></a>
## clear()
#### Description
The method removes all events currently stored in the queue and makes it empty.

#### Syntax
    void clear();

#### Parameters
None

#### Returns
None

#### See also
[Back to interface](#interface)


<a id="publish"></a>
## publish()
#### Description
The method publishes one event at the front of the queue to the Particle Cloud at the frequency defined by the internal publishing time period parameter.
- The default publishing time period parameter is defined by the macro [QUEUEPROCESSING\_PUBLISH\_PERIOD\_DEF](#Constants), which is usually 1 second. This default time period can be changed by the setter [setPublishPeriod()](#setPublishPeriod).
- The method is intended to run in a loop of an application and publish queued events one by one evenly.
- If the publishing is unsuccessful, e.g., due to disconnecting from the Particle Cloud, the method leaves particular event in the queue and tries to publish it in the next run. Otherwise at successful publishing, the method removes that event from the queue.

#### Syntax
    bool publish();

#### Parameters
None

#### Returns
Logical flag about individual publishing success.

#### See also
[publishBatch()](#publishBatch)

[Back to interface](#interface)


<a id="publishBatch"></a>
## publishBatch()
#### Description
The method publishes a batch of top events in the queue to the Particle Cloud at once at the frequency defined by the internal publishing time period parameter multiplied by number of events in the batch, which results usually to 4 seconds.
- The default publishing time period parameter is defined by the macro [QUEUEPROCESSING\_PUBLISH\_PERIOD\_DEF](#Constants), which is usually 1 second. This default time period can be changed by the setter [setPublishPeriod()](#setPublishPeriod).
- The default number of events in a batch is defined by the macro [QUEUEPROCESSING\_PUBLISH\_COUNT\_DEF](#Constants), which is usually 4. This default number can be changed by the setter [setPublishBatch()](#setPublishBatch).
- This method is intended to run preferrably in a timer and publish queued events in bursts. However, it can be used in a loop of an application as well.
- If the publishing of some event in a batch is unsuccessful, the method leaves that event in the queue, breakes the publishing batch, and tries to publish it along side with rest of event in the original batch in the next batch. Successfully published events in a batch are removed from the queue by the method individually.

#### Syntax
    bool publishBatch();

#### Parameters
None

#### Returns
Logical flag about batch publishing success.

#### See also
[publish()](#publish)

[Back to interface](#interface)


<a id="setPublishPeriod"></a>
## setPublishPeriod()
#### Description
The method defines the publishing time period for publishing individual events.
- The method overrides the current value.

#### Syntax
    void setPublishPeriod(uint16_t period);

#### Parameters
- **period**: The publishing period in milliseconds.
  - *Valid values*: positive integer equal or greater than [QUEUEPROCESSING\_PUBLISH\_PERIOD\_MIN](#Constants)
  - *Default value*: [QUEUEPROCESSING\_PUBLISH\_PERIOD\_DEF](#Constants)

#### Returns
None

#### See also
[getPublishPeriod()](#getPublishPeriod)

[Back to interface](#interface)


<a id="setPublishBatch"></a>
## setPublishBatch()
#### Description
The method defines the count of events that should be published at once in a publishing burst.
- The method overrides the current value.

#### Syntax
    void setPublishBatch(uint8_t number);

#### Parameters
- **number**: The number of publishing events in a batch.
  - *Valid values*: positive short integer within the range [QUEUEPROCESSING\_PUBLISH\_COUNT\_MIN](#Constants) ~ [QUEUEPROCESSING\_PUBLISH\_COUNT\_MAX](#Constants)
  - *Default value*: [QUEUEPROCESSING\_PUBLISH\_COUNT\_DEF](#Constants)

#### Returns
None

#### See also
[getPublishBatch()](#getPublishBatch)

[Back to interface](#interface)


<a id="isEmpty"></a>
## isEmpty()
#### Description
The method returns the flag whether the queue is empty.

#### Syntax
    bool isEmpty();

#### Parameters
None

#### Returns
Flag about queue emptyness.

#### See also
[Back to interface](#interface)


<a id="getQueueLen"></a>
## getQueueLen()
#### Description
The method returns the length of the queue counting in number of events currently present in the queue.

#### Syntax
    uint16_t getQueueLen();

#### Parameters
None

#### Returns
Number of events in the queue.

#### See also
[Back to interface](#interface)













<a id="getPublishPeriod"></a>
## getPublishPeriod(), getPublishBatch()
#### Description
The methods are getters of corresponding setters and return current internal processing parameters after appropriate sanitizing their values.

#### Syntax
    uint16_t getPublishPeriod();
    uint8_t  getPublishBatch();

#### Parameters
None

#### Returns
Particular internal processing parameter.

#### See also
[setPublishPeriod()](#setPublishPeriod)

[setPublishBatch()](#setPublishBatch)

[Back to interface](#interface)
