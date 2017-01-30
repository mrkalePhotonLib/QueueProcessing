#include <queue-processing.h>

// Constructor
QueueProcessing::QueueProcessing() {}


// Enqueue event by parameters
void QueueProcessing::enqueue(String eventName, String eventText, uint8_t eventScope)
{
    // Sanitize event name and text
    eventName = eventName.substring(0, 63);
    eventText = eventText.substring(0, 255);

    // Sanitize scope
    switch (eventScope)
    {
        case QUEUEPROCESSING_PUBLISH_PUBLIC:
        case QUEUEPROCESSING_PUBLISH_PRIVATE:
            break;
        default:
            eventScope = QUEUEPROCESSING_PUBLISH_PRIVATE;
            break;
    }

    QueueProcessingEvent event = {.name = eventName, .text = eventText, .scope = eventScope};
    _queue.push(event);
}


// Enqueue event by structure
void QueueProcessing::enqueue(QueueProcessingEvent event)
{
    enqueue(event.name, event.text, event.scope);
}


QueueProcessingEvent QueueProcessing::dequeue()
{
    QueueProcessingEvent event;
    event = _queue.front();
    _queue.pop();
    return event;
}


// Get top event without removing it
QueueProcessingEvent QueueProcessing::peek()
{
    return _queue.front();
}


String QueueProcessing::peekName()
{
    QueueProcessingEvent event;
    event = _queue.front();
    return event.name;
}


String QueueProcessing::peekText()
{
    QueueProcessingEvent event;
    event = _queue.front();
    return event.text;
}


uint8_t QueueProcessing::peekScope()
{
    QueueProcessingEvent event;
    event = _queue.front();
    return event.scope;
}


void QueueProcessing::clear()
{
    _queue = std::queue<QueueProcessingEvent>();
}


//  Publish the top event to the Particle Cloud
bool QueueProcessing::publish()
{
    if (millis() - _tsPublish >= _publishPeriod && !_queue.empty())
    {
        _tsPublish = millis();
        QueueProcessingEvent event = _queue.front();
        bool publishSuccess;
        switch (event.scope)
        {
            case QUEUEPROCESSING_PUBLISH_PUBLIC:
                publishSuccess = Particle.publish(event.name, event.text);
                break;

            case QUEUEPROCESSING_PUBLISH_PRIVATE:
                publishSuccess = Particle.publish(event.name, event.text, PRIVATE);
                break;

            default:
                publishSuccess = false;
                break;
        }
        if (publishSuccess) _queue.pop();
        return publishSuccess;
    }
    return true;
}


//  Publish batch of top event to the Particle Cloud
bool QueueProcessing::publishBatch()
{
    if (millis() - _tsPublish >= (_publishPeriod * _publishBatch) && !_queue.empty())
    {
        _tsPublish = millis();
        bool publishSuccess = true;
        for (uint8_t i = 0; i < _publishBatch && !_queue.empty(); i++)
        {
            QueueProcessingEvent event = _queue.front();
            switch (event.scope)
            {
                case QUEUEPROCESSING_PUBLISH_PUBLIC:
                    publishSuccess = Particle.publish(event.name, event.text);
                    break;

                case QUEUEPROCESSING_PUBLISH_PRIVATE:
                    publishSuccess = Particle.publish(event.name, event.text, PRIVATE);
                    break;

                default:
                    publishSuccess = false;
                    break;
            }
            if (publishSuccess)
            {
                _queue.pop();
            }
            else
            {
                break;
            }
        }
        return publishSuccess;
    }
    return true;
}


//-------------------------------------------------------------------------
// Setters
//-------------------------------------------------------------------------

void QueueProcessing::setPublishPeriod(uint16_t period)
{
    _publishPeriod = max(period, QUEUEPROCESSING_PUBLISH_PERIOD_MIN);
}


void QueueProcessing::setPublishBatch(uint8_t number)
{
    _publishBatch = constrain(number, QUEUEPROCESSING_PUBLISH_COUNT_MIN, QUEUEPROCESSING_PUBLISH_COUNT_MAX);
}


//-------------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------------
bool     QueueProcessing::isEmpty() { return _queue.empty(); }
uint16_t QueueProcessing::getQueueLen() { return _queue.size(); }
uint16_t QueueProcessing::getPublishPeriod() { return _publishPeriod; }
uint8_t  QueueProcessing::getPublishBatch() { return _publishBatch; }
