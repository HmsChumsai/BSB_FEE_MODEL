MY_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source=MY_DIR/import_fee_model.cfg



con --play $MY_DIR/ExportCluster.fub --tcdisplay NULLDISPLAY --role ssmkorn
awk -F, 'NR>1 {print $2}' /tmp/cluster_name.csv >> /tmp/tfex_cluster_name.txt

 
