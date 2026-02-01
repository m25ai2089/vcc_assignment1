# VirtualBox 2-VM C++ Microservices (Gateway + Inventory)

| Field | Value |
|------|-------|
| Student Name | Chaitan Divagi |
| Roll No. | M25AI2089 |
| Course / Subject | CSL7510 VCC |
| Assignment | Assignment 1 – VirtualBox: 2 VMs + Microservice Deployment |
| Institute | IIT Jodhpur |
| Date | 01 Feb 2026 |

## Objective
Create two VirtualBox VMs, connect them using a private host-only network, and deploy two small C++ HTTP microservices where **VM-A (Gateway)** calls **VM-B (Inventory)**.

## VM Roles and IPs
### VM-A (Gateway Service)
- IP: `192.168.56.5`
- Port: `8080`
- Endpoints:
  - `GET /health`
  - `GET /proxy-items`

### VM-B (Inventory Service)
- IP: `192.168.56.4`
- Port: `8081`
- Endpoints:
  - `GET /health`
  - `GET /items`

## Network Setup (per VM)
- Adapter 1: **NAT** (internet access for apt installs)
- Adapter 2: **Host-only** (private VM-to-VM communication, `192.168.56.0/24`)

## Repository Structure
```
.
├── inventory_service/
│   ├── main.cpp
│   └── httplib.h
└── gateway_service/
    ├── main.cpp
    └── httplib.h
```

## Prerequisites (run on both VMs)
```bash
sudo apt update
sudo apt install -y build-essential
```

## Static IP (Host-only)
- VM-A: `192.168.56.5/24`
- VM-B: `192.168.56.4/24`

Note: Static IP ensures the IP does not change after reboot.

## Build and Run (No Scripts)

### 1) Inventory Service on VM-B (Port 8081)
```bash
ssh ubuntu@192.168.56.4
cd ~/inventory_service
g++ -std=c++17 main.cpp -o inventory_service
./inventory_service &
```

Test on VM-B:
```bash
curl http://127.0.0.1:8081/health
curl http://127.0.0.1:8081/items
```

### 2) Gateway Service on VM-A (Port 8080)
```bash
ssh ubuntu@192.168.56.5
cd ~/gateway_service
g++ -std=c++17 main.cpp -o gateway_service
./gateway_service &
```

Test on VM-A:
```bash
curl http://127.0.0.1:8080/health
curl http://127.0.0.1:8080/proxy-items
```

## Verification (Proof of Communication)

### A) VM-A reaches Inventory directly
```bash
ssh ubuntu@192.168.56.5
curl http://192.168.56.4:8081/items
```

### B) Gateway calls Inventory (service-to-service)
```bash
curl http://127.0.0.1:8080/proxy-items
```

### C) Full chain from host machine (Host → VM-A → VM-B → VM-A)
```bash
curl http://192.168.56.5:8080/proxy-items
```

## Troubleshooting (quick checks)

Check ports listening:
```bash
# On VM-B
sudo ss -tulpn | grep 8081

# On VM-A
sudo ss -tulpn | grep 8080
```

Check processes:
```bash
ps -ef | grep inventory_service
ps -ef | grep gateway_service
```

## Video Demo
https://www.youtube.com/watch?v=m_MIbvozs_4

## Conclusion
Two connected Ubuntu ARM VMs were created in VirtualBox. Inventory service runs on VM-B (8081) and Gateway service runs on VM-A (8080). Gateway successfully communicates with Inventory over the host-only network, demonstrating a working microservice deployment across two VMs.
