# ITU ACM - Systems Programming Workshop

## Course Overview
This workshop, offered by ITU ACM, is designed to introduce participants to the fundamentals of systems programming. Covering topics from device driver I/O to process management, the workshop aims to provide hands-on experience with kernel modules, file systems, and process management through a series of projects.

## Lecturer's Contacts
- **Name:** Hayri Turgut Uyar
- **Email:** uyar at itu.edu.tr
- **Matrix/Element:** @uyar:matrix.org

A Matrix chat group will be created to keep in touch, share updates, and help each other out throughout the course. It's also a direct line to the lecturer for any questions you've got.

Reach out to one of the responsible students or the lecturer, to join the group.

## Schedule

| Date       | Day | Topics         | Location       |
|------------|-----|----------------|----------------|
| 2024-03-04 | Mon | Introduction   | EEB 5101       |
| 2024-03-11 | Mon |                |                |
| 2024-03-18 | Mon |                |                |
| 2024-03-25 | Mon |                |                |
| 2024-04-01 | Mon |                |                |
| 2024-04-08 | Mon |                |                |
| 2024-04-15 | Mon |                |                |
| 2024-04-22 | Mon |                |                |
| 2024-04-29 | Mon |                |                |


## Location
- **Building:** Electrics and Electronics Building (EEB) 5101

## Time
- **Duration:** 18:00-20:00

## Course Outline

| Order | Project            | Approach       |
|-------|--------------------|----------------|
| 1     | Device driver I/O  | Kernel module  |
| 2     | File system (FUSE) | User-space     |
| 3     | Process management | Kernel compile |

## Prerequisites
Participants should have a basic understanding of operating systems and C programming. Prior experience with Linux or Unix-like environments is recommended.

## Setting Up Your Development Environment
To ensure a safe and uniform development environment, we recommend using a virtual machine. This helps prevent any potential harm to your computer's operating system. We will be using Oracle VM VirtualBox and Ubuntu 22.04 LTS (Jammy Jellyfish).

### Steps to Set Up Your Virtual Machine

1. **Download and Install Oracle VM VirtualBox**
   - Visit [Oracle VM VirtualBox](https://www.virtualbox.org/) and download the latest version of VirtualBox for your operating system. Follow the installation instructions provided on the website.

2. **Download Ubuntu 22.04 LTS**
   - Download Ubuntu 22.04 LTS from the [Official Ubuntu Website](https://releases.ubuntu.com/22.04/). Choose the ISO file for the desktop version.

3. **Create a New Virtual Machine**
   - Open VirtualBox and click "New" to create a new virtual machine.
   - Name your virtual machine (e.g., "Ubuntu 22.04 LTS Workshop") and select "Linux" as the type and "Ubuntu (64-bit)" as the version.
   - Allocate at least **4 GB of RAM** and create a new virtual hard disk with **at least 40 GB** of space to ensure there is enough storage for the final project, where we will compile the Linux kernel. A single CPU Core should be enough.

4. **Install Ubuntu on Your Virtual Machine**
   - Follow the VirtualBox prompts to start the virtual machine using the Ubuntu ISO file you downloaded. Proceed with the installation of Ubuntu.
   - Choose English as the installation Language.
   - Opt for a **Minimal Installation** during the setup process, which excludes office software, games, and other unnecessary applications.

5. **Install VirtualBox Guest Additions**
   - Once Ubuntu is installed, install the VirtualBox Guest Additions for improved integration between the host system and the virtual machine (e.g., better screen resolution, shared clipboard, shared folders, drag and drop option, etc.).

## Lecture Notes
Lecture notes under ```unofficialLectureNotes``` are provided by [Denis Davidoglu](https://github.com/dawidogg).