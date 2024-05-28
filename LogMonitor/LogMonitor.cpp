#include <pcapplusplus/PcapLiveDeviceList.h>
#include <pcapplusplus/PcapLiveDevice.h>
#include <pcapplusplus/PcapPlusPlusVersion.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/TcpLayer.h>
#include <pcapplusplus/SystemUtils.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <arpa/inet.h>

bool keepRunning = true;
static uint16_t destination_port = 51400;

void onApplicationInterrupted(int signal) {
    keepRunning = false;
}

void packetHandler(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie) {
    pcpp::Packet parsedPacket(packet);
    if (parsedPacket.isPacketOfType(pcpp::TCP)) {
        pcpp::TcpLayer* tcpLayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
        if (tcpLayer != nullptr) {
            uint16_t dstPort = ntohs(tcpLayer->getTcpHeader()->portDst);
            if (dstPort == destination_port) {
                std::string payload((char*)tcpLayer->getLayerPayload(), tcpLayer->getLayerPayloadSize());
                std::cout << payload << std::flush;
            }
        }
    }
}

void startCapture(pcpp::PcapLiveDevice* dev) {
    if (!dev->open()) {
        std::cerr << "Cannot open device" << std::endl;
        return;
    }

    dev->startCapture(packetHandler, nullptr);

    std::cout << "Listening on port "<< destination_port << "!" <<std::endl;

    while (keepRunning) {
        pcpp::multiPlatformSleep(1);
    }

    dev->stopCapture();
    dev->close();
}

int main(int argc, char* argv[]) {
    signal(SIGINT, onApplicationInterrupted);

    pcpp::PcapLiveDevice* dev = nullptr;

    while (keepRunning) {
        if (dev == nullptr) {
            dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp("127.0.0.1");
            if (dev == nullptr) {
                std::cerr << "Cannot find device, retrying in 5 seconds..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }
        }

        if (!dev->open()) {
            std::cerr << "Cannot open device, retrying in 5 seconds..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        startCapture(dev);
    }

    return 0;
}