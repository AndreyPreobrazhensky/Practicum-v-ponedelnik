#include <iostream>
#include <vector>

class CSR_graph {
    int row_count; //number of vertices in graph
    unsigned int col_count; //number of edges in graph
    
    std::vector<unsigned int> row_ptr;
    std::vector<int> col_ids;
    std::vector<double> vals;

public:

    void read(const char* filename) {
        FILE *graph_file = fopen(filename, "rb");
        fread(reinterpret_cast<char*>(&row_count), sizeof(int), 1, graph_file);
        fread(reinterpret_cast<char*>(&col_count), sizeof(unsigned int), 1, graph_file);

        std::cout << "Row_count = " << row_count << ", col_count = " << col_count << std::endl;
        
        row_ptr.resize(row_count + 1);
        col_ids.resize(col_count);
        vals.resize(col_count);
         
        fread(reinterpret_cast<char*>(row_ptr.data()), sizeof(unsigned int), row_count + 1, graph_file);
        fread(reinterpret_cast<char*>(col_ids.data()), sizeof(int), col_count, graph_file);
        fread(reinterpret_cast<char*>(vals.data()), sizeof(double), col_count, graph_file);
        fclose(graph_file);
    }

    int weightMAX(){
	    double count, maxcount = 0; 
	    int maxid = 0;
	    for (int i = 0; i < row_ptr.size() - 1; i++){
		count = 0;
	        for (int col = row_ptr[i]; col < row_ptr[i + 1]; col++){
			if (col_ids[col] % 2 == 0){
				count += vals[col];
			}
		}
		if (count > maxcount){
			maxid = i;
			maxcount = count;
		}
	    }
	    std::cout << maxcount << std::endl;
	    return maxid;
    }    
    int weightMAXver(){
            double count, maxcount = 0;         
            int maxid = 0;
            for (int i = 0; i < row_ptr.size() - 1; i++){
                count = 0;
                for (int col = row_ptr[i]; col < row_ptr[i + 1]; col++){
			double w = 0;
			for (int j = row_ptr[col_ids[col]]; j < row_ptr[col_ids[col] + 1]; j++){
			       w += vals[j] * (row_ptr[col_ids[j] + 1] - row_ptr[col_ids[j]]);
			}	       
                        count += vals[col] * w;
                }
                if (count > maxcount){
                        maxid = i;
                        maxcount = count;
                }
            }
            std::cout << maxcount << std::endl;
            return maxid;
    }


    void print_vertex(int idx) {
        for (int col = row_ptr[idx]; col < row_ptr[idx + 1]; col++) {
            std::cout << col_ids[col] << " " << vals[col] <<std::endl;
        }
        std::cout << std::endl;
    }

    void reset() {
        row_count = 0;
        col_count = 0;
        row_ptr.clear();
        col_ids.clear();
        vals.clear();
    }
}; 

#define N_TESTS 5

int main () {
    const char* filenames[N_TESTS];
    filenames[0] = "synt";
    filenames[1] = "road_graph";
    filenames[2] = "stanford";
    filenames[3] = "youtube";
    filenames[4] = "syn_rmat";

    for (int n_test = 0; n_test < N_TESTS; n_test++) {
        CSR_graph a;
        a.read(filenames[n_test]);
	std::cout << a.weightMAX() << std::endl;
	std::cout << "max weight ver:" << a.weightMAXver() << std::endl;
        

        //


    }
    return 0;
}
