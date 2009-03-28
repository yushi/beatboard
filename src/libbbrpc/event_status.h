#ifndef EventStatus_H
#define EventStatus_H

namespace BeatBoard {
  class EventStatus {
  private:
    void* instance;
    int status;
    void* client_event_status;

  public:
    EventStatus( void* instance, int status, void* client_event_status );
    virtual ~EventStatus();

    void* getInstance();
    int getStatus();
    void* getClientEventStatus();
  };
}
#endif
