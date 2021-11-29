#!/bin/bash

#Exo 2
exo2=exo2-client
exo3=exo3-client
if test -f "$exo2"; then
	# On crée plusieurs instances de client qui vont soliciter le serveur
	for i in {1..20}
	do
		./$exo2 $i&
	done
elif test -f "$exo3"; then #Exo 3
	echo "test de +"
	for i in {1..20}
	do
		./$exo3 $i + $((i*2)) &
	done
	echo "test de -"
	for i in {1..20}
	do
		./$exo3 $i - $((i*2)) &
	done
	echo "test de *"
	for i in {1..20}
	do
		./$exo3 $i "*" $((i*2)) &
	done
	echo "test de /"
	for i in {1..20}
	do
		./$exo3 $((i*2)) / $i &
	done
fi

