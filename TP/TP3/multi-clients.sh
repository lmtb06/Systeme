#!/bin/bash

exo2=exo2-client
exo3=exo3-client
exo4=exo4-client
#Exo 2
if test -f "$exo2"; then
	# On crée plusieurs instances de client qui vont soliciter le serveur
	for i in {1..20}
	do
		./$exo2 $i&
	done
elif test -f "$exo3" || test -f "$exo4"; then #Exo 3 et exo 4
	exo_courant="$exo3"
	if test -f "$exo4"; then
		exo_courant="$exo4"
	fi

	# echo "test de +"
	for i in {1..20}
	do
		./$exo_courant $i + $((i*2)) &
	done

	# echo "test de *"
	for i in {1..20}
	do
		./$exo_courant $i "*" $((i*2)) &
	done
	
	# echo "test de -"
	for i in {1..20}
	do
		./$exo_courant $i - $((i*2)) &
	done


	# echo "test de /"
	for i in {1..20}
	do
		./$exo_courant $((i*2)) / $i &
	done
fi

