#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ================= Decorator =================

class INotification {
public:
    virtual ~INotification() {}
    virtual string getContent(string message) = 0;
};

class SimpleNotification : public INotification {
public:
    string getContent(string message) override {
        return message;
    }
};

class INotificationDecorator : public INotification {
protected:
    INotification* notification_;
public:
    INotificationDecorator(INotification* notification)
        : notification_(notification) {}

    string getContent(string message) override {
        return notification_->getContent(message);
    }
};

class TimestampDecorator : public INotificationDecorator {
public:
    TimestampDecorator(INotification* notification)
        : INotificationDecorator(notification) {}

    string getContent(string message) override {
        return "[Time] " + notification_->getContent(message);
    }
};

class SignatureDecorator : public INotificationDecorator {
public:
    SignatureDecorator(INotification* notification)
        : INotificationDecorator(notification) {}

    string getContent(string message) override {
        return notification_->getContent(message) + " - Signature";
    }
};

// ================= Observer =================

class IObservers {
public:
    virtual ~IObservers() {}
    virtual void Update(string message) = 0;
};

class User : public IObservers {
    string name;
public:
    User(string name) : name(name) {}

    void Update(string message) override {
        cout << name << " received: " << message << endl;
    }
};

class IObservable {
public:
    virtual void registerObserver(IObservers* observer) = 0;
    virtual void removeObserver(IObservers* observer) = 0;
    virtual void notifyObservers(string message) = 0;
};

class NotificationObservable : public IObservable {
    vector<IObservers*> observers;
    INotification* notification;
public:
    NotificationObservable(INotification* notification)
        : notification(notification) {}

    void registerObserver(IObservers* observer) override {
        observers.push_back(observer);
    }

    void removeObserver(IObservers* observer) override {
        // simple remove
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers(string message) override {
        string finalMessage = notification->getContent(message);
        for (auto obs : observers) {
            obs->Update(finalMessage);
        }
    }
};

// ================= Main =================

int main() {
    INotification* base = new SimpleNotification();
    base = new TimestampDecorator(base);
    base = new SignatureDecorator(base);

    NotificationObservable system(base);

    IObservers* user1 = new User("Ganesh");
    IObservers* user2 = new User("Alex");

    system.registerObserver(user1);
    system.registerObserver(user2);

    system.notifyObservers("Hello World");

    return 0;
}