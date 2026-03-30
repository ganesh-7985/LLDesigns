/**
 * Intent: Define a Subscription Mechansim to notify multiple objects about any event 
 * that happens to the object they're observing
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>  

using namespace std;

// ================= INTERFACES =================

class ISubscriber {
public:
    virtual ~ISubscriber() {}
    virtual void Update(const string& message_from_publisher) = 0;
};

class IPublisher {
public:
    virtual ~IPublisher() {}
    virtual void Attach(ISubscriber* subscriber) = 0;
    virtual void Detach(ISubscriber* subscriber) = 0;
    virtual void Notify() = 0;
};

// ================= PUBLISHER =================

class Publisher : public IPublisher {
private:
    vector<ISubscriber*> subscribers_list;
    string message;

public:
    ~Publisher() {
        cout << "GoodBye, I was Publisher" << endl;
    }

    void Attach(ISubscriber* subscriber) override {
        subscribers_list.push_back(subscriber);
    }

    void Detach(ISubscriber* subscriber) override {
        subscribers_list.erase(
            remove(subscribers_list.begin(), subscribers_list.end(), subscriber),
            subscribers_list.end()
        );
    }

    void Notify() override {
        for (ISubscriber* subscriber : subscribers_list) {
            subscriber->Update(message);
        }
    }

    void create_message(string message) {
        this->message = message;
        Notify();
    }

    void SomeBusinessLogic() {
        this->message = "change message";
        Notify();
        cout << "I am about to do something" << endl;
    }
};

// ================= SUBSCRIBER =================

class Subscriber : public ISubscriber {
private:
    string message_from_publisher;
    int number_;
    Publisher& publisher_;
    static int static_number_;

public:
    Subscriber(Publisher& publisher) : publisher_(publisher) {
        publisher_.Attach(this);
        number_ = ++static_number_;
        cout << "Hi, I am subscriber " << number_ << endl;
    }

    ~Subscriber() {
        cout << "GoodBye, I was Subscriber " << number_ << endl;
    }

    void Update(const string& message_from_publisher) override {
        this->message_from_publisher = message_from_publisher;
        PrintInfo();
    }

    void RemoveMeFromList() {
        publisher_.Detach(this);
        cout << "Subscriber " << number_ << " removed from list" << endl;
    }

    void PrintInfo() {
        cout << "Subscriber " << number_
             << " got message: " << message_from_publisher << endl;
    }
};

int Subscriber::static_number_ = 0;

// ================= CLIENT =================

void clientCode() {
    Publisher* publisher = new Publisher;

    Subscriber* s1 = new Subscriber(*publisher);
    Subscriber* s2 = new Subscriber(*publisher);
    Subscriber* s3 = new Subscriber(*publisher);

    publisher->create_message("Hello World! :D");

    s3->RemoveMeFromList();

    publisher->create_message("Weather is so HOT! :D");

    Subscriber* s4 = new Subscriber(*publisher);

    s2->RemoveMeFromList();

    Subscriber* s5 = new Subscriber(*publisher);

    publisher->create_message("My new car is great");

    s5->RemoveMeFromList();
    s4->RemoveMeFromList();
    s1->RemoveMeFromList();

    delete s1;
    delete s2;
    delete s3;
    delete s4;
    delete s5;
    delete publisher;
}

int main() {
    clientCode();
    return 0;
}