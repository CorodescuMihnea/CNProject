all :  
	@echo "Please choose server or client (i.e. make serv/make cli)"
	@echo "Nothing done"

serv:
	@echo "Building server"
	cd server; make all; cd ..;

cli:
	@echo "Building client"
	cd client; make all; cd ..;

clean:
	@echo "Please choose server or client (i.e. make cleanS/make cleanC"
	@echo "Nothing done"

cleanS:
	@echo "Cleaning server"
	cd server; make clean; cd ..;
	@echo "Done cleaning server"

cleanC:
	@echo "Cleaning client"
	cd client; make clean; cd ..;
	@echo "Done cleaning client"
	