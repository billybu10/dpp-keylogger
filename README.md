# dpp-keylogger
linux keylogger that sends data through webhook to a discord channel

Heavily based on: [this repo](https://github.com/SCOTPAUL/keylog)

## How to compile and run

Before compilation you have to get a webhook to channel and put it into webhook.h. [Here's how to get one](https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks)

```bash
  make keylog
  make clean
  ./keylog
```

## Disclaimer
This program was made as an exercise and shouldn't be used on devices not owned by you and on users without their knowledge and consent.
I do not encourage nor condone using my code for malicious purpose. Also, note that using discord might make finding your data easier.
