# VirtualBox 2-VM C++ Microservices (Gateway + Inventory)

| Field | Value |
|------|-------|
| Student Name | Chaitan Divagi |
| Roll No. | M25AI2089 |
| Course / Subject | CSL7510 VCC |
| Assignment | Assignment 1 – VirtualBox: 2 VMs + Microservice Deployment |
| Institute | IIT Jodhpur |
| Date | 01 Feb 2026 |

---

## Objective
Create and configure two VirtualBox virtual machines (VMs), connect them through a private network, and deploy a simple microservice-based application across the connected VMs.

---

## Architecture Overview

### VM Roles
- **VM-A (Gateway Service)**
  - IP: `192.168.56.5`
  - Service: `gateway_service`
  - Port: `8080`
  - Endpoint: `GET /proxy-items` (calls Inventory service and returns combined JSON)

- **VM-B (Inventory Service)**
  - IP: `192.168.56.4`
  - Service: `inventory_service`
  - Port: `8081`
  - Endpoint: `GET /items` (returns JSON item list)

### Network Configuration
Each VM uses:
- **Adapter 1: NAT** (internet access for installing packages)
- **Adapter 2: Host-only** (private VM-to-VM communication)

macOS Host (VirtualBox)
  |
Host-only Network (192.168.56.0/24)
  |
  +-- VM-A 192.168.56.5 :8080  (gateway_service)
  |        |
  |        +--> HTTP call to VM-B 192.168.56.4 :8081 (/items)
  |
  +-- VM-B 192.168.56.4 :8081  (inventory_service)

---

## Repository Structure
.
├── inventory_service/
│   ├── main.cpp
│   └── httplib.h
└── gateway_service/
    ├── main.cpp
    └── httplib.h

---

## Prerequisites
- VirtualBox installed on host machine
- Two Ubuntu ARM VMs created and connected using:
  - NAT (Adapter 1)
  - Host-only (Adapter 2)
- SSH enabled (recommended)
- On each VM:
  - g++ available

Install build tools on each VM:
sudo apt update
sudo apt install -y build-essential

---

## Static IP Setup (Host-only)
- VM-A: 192.168.56.5/24
- VM-B: 192.168.56.4/24

Note: Static IP ensures the IP does not change after reboot.

---

## Build & Run (No Scripts)

### 1) Inventory Service (VM-B) – Port 8081

SSH into VM-B:
ssh ubuntu@192.168.56.4

Go to service directory:
cd ~/inventory_service

Build:
g++ -std=c++17 main.cpp -o inventory_service

Run:
./inventory_service &

Test (on VM-B):
curl http://127.0.0.1:8081/health
curl http://127.0.0.1:8081/items

---

### 2) Gateway Service (VM-A) – Port 8080

SSH into VM-A:
ssh ubuntu@192.168.56.5

Go to service directory:
cd ~/gateway_service

Build:
g++ -std=c++17 main.cpp -o gateway_service

Run:
./gateway_service &

Test (on VM-A):
curl http://127.0.0.1:8080/health
curl http://127.0.0.1:8080/proxy-items

---

## Verification (Proof of Inter-VM Microservice Communication)

### A) VM-A can reach Inventory service on VM-B (direct call)
Run on VM-A:
ssh ubuntu@192.168.56.5
curl http://192.168.56.4:8081/items

### B) Gateway calls Inventory (service-to-service communication)
Run on VM-A:
curl http://127.0.0.1:8080/proxy-items

Expected output structure:
{
  "service": "gateway",
  "data": {
    "service": "inventory",
    "items": [
      {"id": 1, "name": "apple"},
      {"id": 2, "name": "banana"}
    ]
  }
}

### C) Full chain test (Host → VM-A → VM-B → VM-A)
Run from host machine (macOS terminal):
curl http://192.168.56.4:8080/proxy-items

If the response is returned successfully, it confirms end-to-end inter-VM microservice communication.

---

## Troubleshooting

### Check ports listening (Ubuntu)
Inventory (VM-B):
sudo ss -tulpn | grep 8081

Gateway (VM-A):
sudo ss -tulpn | grep 8080

### Common issue: “inventory unreachable”
- Ensure inventory service is running on VM-B
- Ensure inventory service listens on 0.0.0.0:8081 (not only localhost)
- Ensure gateway uses correct IP 192.168.56.11:8081

---

## Video Demo
Video link: https://www.youtube.com/watch?v=m_MIbvozs_4

Suggested demo checklist:
1. Show VirtualBox network settings (NAT + Host-only)
2. Show both VMs and their IP addresses (ip a)
3. Ping VM-A ↔ VM-B
4. Start inventory service on VM-B and test /items
5. Start gateway service on VM-A and test /proxy-items
6. Run final end-to-end curl test from host

---

## Conclusion
Two connected VMs were successfully created and configured in VirtualBox. A C++ inventory microservice was deployed on VM-B and a C++ gateway microservice was deployed on VM-A. The gateway service successfully communicated with the inventory service over the private host-only network, demonstrating a working microservice-based deployment across multiple VMs.
