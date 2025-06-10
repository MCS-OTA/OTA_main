# OTA_main

## Introduction

This project addresses the critical security vulnerabilities in Over-the-Air(OTA) updates for modern vehicles, which have become essential "driving computers" requiring frequent software and firmware updates. While OTA technoogy enables convenient wireless updates directly from servers to vehicle systems, it faces significant cybersecurity threats including man-in-the-middle attacks, firmware spoofing, and unauthorized access that compromise vehicle safety and user privacy. Our research develops comprehensive security countermeasures throgh advanced encryption, digital signatures, and secure boot process implemented across network, system, and application levels, providing the automotive industry with essential tools to effectively combat emerging cyber threats and ensure secure software deployment.

## Architecture
![Image](https://github.com/user-attachments/assets/135ad3f1-07e7-4158-a0ff-700e56d0cad1)

## Process
### Download
![Image](https://github.com/user-attachments/assets/361b5396-5575-4b78-84e2-3feb8dd18587)

### Installation
![Image](https://github.com/user-attachments/assets/25ed4ab2-787e-4e8e-bf9a-2fbcdbd206ae)

## Attack Scenarios
### ARP Spoofing for Fake Server Redirection

**Precondition**
- Assume that the attacker has access to the network where the OTA update is taking place

**Attack Procedure**
1. Attacker performs ARP spoofing and IP forwarding to gain a man-in-the-middle position between broker and client.
2. Intercept client's download URL request and redirects to malicious URL
3. Download a malicious image on the client side

**Security Plan**
- Ensuring confidentiality through TLS and strengthening data integrity through digital signatures
- Plan to explore ways to ensure trustworthiness of certificates and keys

### Inducing abnomal rollback through replay attack

**Precondition**
- Assume that the attacker know the broker IP and Client IP, and can intercept the OTA update packets

**Attack Procedure**
1. Capture vulnerable message packets through sniffing
2. Retransmit captured packets by posing as a fake broker
3. Download a vulnerable older version on the client side

**Security Plan**
- Check the expire time by verifying the timestamp
- Prevent an abnormal rollback by managing version list
## Developers

<table align="center">

  <tr>
    <td align="center">
      <a href="https://github.com/YEOHANBIN">
        <img src="https://github.com/YEOHANBIN.png" width="150px;" alt="Hanbin Yeo"/>
        <br />
        <sub><b>Hanbin Yeo</b></sub>
      </a>
      <br />
      <a href="https://github.com/YEOHANBIN"><img src="https://img.shields.io/badge/GitHub-YEOHANBIN-blue?logo=github" alt="GitHub Badge" /></a>
      <br />
    </td>
    <td align="center">
      <a href="https://github.com/jwoon0906">
        <img src="https://github.com/jwoon0906.png" width="150px;" alt="Jangwoon Park"/>
        <br />
        <sub><b>Jangwoon Park</b></sub>
      </a>
      <br />
      <a href="https://github.com/jwoon0906"><img src="https://img.shields.io/badge/GitHub-jwoon0906-blue?logo=github" alt="GitHub Badge" /></a>
      <br />
    </td>
    <td align="center">
      <a href="https://github.com/KunhoPark-Jason">
        <img src="https://github.com/KunhoPark-Jason.png" width="150px;" alt="Kunho Park"/>
        <br />
        <sub><b>Kunho Park</b></sub>
      </a>
      <br />
      <a href="https://github.com/KunhoPark-Jason"><img src="https://img.shields.io/badge/GitHub-KunhoPark-Jason-blue?logo=github" alt="GitHub Badge" /></a>
      <br />
    </td>
    
  </tr>
</table>
