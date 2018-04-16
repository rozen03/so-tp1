make clean
make exp
echo "p_archivos,ej5,ej6" >tiempos.csv
for value in {1..60}
do
	printf "\rEjecución: $value"
	./exp 2>>tiempos.csv
done
printf "\n"
