FROM	debian:stable-slim

RUN		apt-get update && yes | apt-get upgrade && yes | apt-get install libncursesw5-dev cmake gcc locales

ADD		.		    			/tmp/

ENTRYPOINT [ "/bin/bash", "/tmp/tools/configure.sh" ]
