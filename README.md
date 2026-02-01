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
Create two VirtualBox VMs, connect them using a private host-only network, and deploy two small C++ HTTP microservices where VM-A (Gateway) calls VM-B (Inventory).

## VM Roles and IPs
### VM-A (Gateway Service)
- IP: 192.168.56.5
- Port: 8080
- Endpoints:
  - GET /health
  - GET /proxy-items

### VM-B (Inventory Service)
- IP: 192.168.56.4
- Port: 8081
- Endpoints:
  - GET /health
  - GET /items

## Network Setup (per VM)
- Adapter 1: NAT (internet access for apt installs)
- Adapter 2: Host-only (private VM-to-VM communication, 192.168.56.0/24)

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
sudo apt install -y build-essential curl
```

## Static IP (Host-only)
- VM-A: 192.168.56.5/24
- VM-B: 192.168.56.4/24

Note: Static IP ensures the IP does not change after reboot.

## Verify Network (Commands + Expected Output)

### 1) Check Host-only IP (on each VM)
Command:
```bash
ip a
```

Expected output:
- On VM-A: line containing `inet 192.168.56.5/24`
- On VM-B: line containing `inet 192.168.56.4/24`

### 2) Ping test (VM-to-VM)
From VM-A:
```bash
ping -c 3 192.168.56.4
```

Expected output contains:
- `3 packets transmitted, 3 received, 0% packet loss`

From VM-B:
```bash
ping -c 3 192.168.56.5
```

Expected output contains:
- `3 packets transmitted, 3 received, 0% packet loss`


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

Expected output (examples):
- /health should return JSON like:
  `{"status":"ok","service":"inventory"}`
- /items should return JSON with an item list, for example:
  `{"service":"inventory","items":[{"id":1,"name":"apple"},{"id":2,"name":"banana"}]}`

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

Expected output (examples):
- /health should return JSON like:
  `{"status":"ok","service":"gateway"}`


## Verification (Proof of Communication)

### A) VM-A reaches Inventory directly
Command (run on VM-A):
```bash
curl http://192.168.56.4:8081/items
```

Expected output:
- Same JSON returned by Inventory /items (item list)

### B) Gateway calls Inventory (service-to-service)
Command (run on VM-A):
```bash
curl http://127.0.0.1:8080/proxy-items
```

Expected output structure (example):
```json
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
```

### C) Full chain from host machine (Host -> VM-A -> VM-B -> VM-A)
Command (run from macOS host terminal):
```bash
curl http://192.168.56.5:8080/proxy-items
```

Expected output:
- Same combined JSON structure returned from the Gateway


## Troubleshooting (quick checks)

Check processes:
```bash
ps -ef | grep inventory_service
ps -ef | grep gateway_service
```

Disable firewall if needed (only for debugging):
```bash
sudo ufw disable
```


## Video Demo
https://www.youtube.com/watch?v=m_MIbvozs_4

## Conclusion
Two connected Ubuntu ARM VMs were created in VirtualBox. Inventory service runs on VM-B (8081) and Gateway service runs on VM-A (8080). Gateway successfully communicates with Inventory over the host-only network, demonstrating a working microservice deployment across two VMs.
