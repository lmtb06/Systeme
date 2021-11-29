#!/bin/bash

# On crée plusieurs instances de client qui vont soliciter le serveur
for i in {1..20}
do
	./exo2-client $i&
done