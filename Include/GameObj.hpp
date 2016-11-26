/* 
 * File:   TestGameObj.hpp
 * Author: dontsov
 *
 * Created on November 26, 2016, 1:29 PM
 */

#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP

#include <vector>

#include "OutputMemoryStream.hpp"
#include "InputMemoryStream.hpp"
#include "TCPSocket.hpp"


class GameObject {
public:
    virtual ~GameObject() {}

    virtual void Update() = 0;
    virtual void Write(OutputMemoryStream& inStream) const = 0;
    virtual void Read(InputMemoryStream& inStream) = 0;
};

class RoboCat : public GameObject {
public:
    RoboCat()
    : mHealth(10)
    , mMeowCount(3)
    , mHomeBase(nullptr) {
        mName[0] = '\0';
    }

    virtual void Update() {
        // TODO
    }
    virtual void Write(OutputMemoryStream& inStream) const {
        inStream.Write(mHealth);
        inStream.Write(mMeowCount);
        // mHomeBase
        inStream.Write(mName, 128);
        // mMiceIndices
    }
    virtual void Read(InputMemoryStream& inStream) {
        inStream.Read(mHealth);
        inStream.Read(mMeowCount);
        // mHomeBase
        inStream.Read(mName, 128);
        // mMiceIndices
    }

private:
    int32_t mHealth;
    int32_t mMeowCount;
    GameObject* mHomeBase;
    char mName[128];
    std::vector<int32_t> mMiceIndices;
};

class Connector {
public:
    void SendGameObj(TCPSocketPtr& inSocket, const GameObject* inGameObject) {
        OutputMemoryStream stream;
        inGameObject->Write(stream);
        inSocket->Send(stream.GetBufferPtr(), stream.GetLength());
    }

    void ReceiveGameObj(TCPSocketPtr& inSocket, GameObject* outGameObject) {
        mInputStream.Clear();
        int32_t receivedByteCount = inSocket->Receive(mInputStream.GetBuffer(), InputMemoryStream::MAX_BUFFER_SIZE);
        if (receivedByteCount > 0) {
            outGameObject->Read(mInputStream);
        }
    }

private:
    InputMemoryStream mInputStream;
};

    #endif /* GAMEOBJ_HPP */