#include <iostream>
#include <cmath>

class Value {

    private:
        double data;        // Internal value of the object

        // Computational node
        enum Operation {
                INIT,
                SUM, SUB, MUL, DIV, 
                POW
            };
        double exponent = 1;
        
        Value** parents; // Array of pointers
        int n_parents;
        Operation op;

        // Optimization
        double gradient = 0;

    public: 
        Value(double data_arg, Value** parents_arg, int n_parents_arg, Value::Operation op_arg) {
            this->data = data_arg;
            this->n_parents = n_parents_arg;
            this->parents = parents_arg;
            this->op = op_arg;
            std::cout << "Generated Value(" << std::to_string(this->data) << ")"<< std::endl;
            
            for(int j=0; j<this->n_parents; j++) {
                std::cout << "\t Parent " << std::to_string(j) << ": " << this->parents[j]->data << std::endl;
            }
            std::cout << "\t Exponent: " << std::to_string(exponent) << std::endl << std::endl;
        }

        Value(double data_arg) :  Value(data_arg, NULL, 0, Value::INIT){
            // Constructor overload
            // I could have used default parameters, but IDC.
        }

    // Backpropagation

    // Clear all gradients from here to the root
    void zero_grad() {
        this->gradient = 0;
        for(int j=0; j<this->n_parents; j++) {
            // This is not very efficient because it will cal zero_grad() on
            // some nodes several times, but IDC
            this->parents[j]->zero_grad();
        }
    }

    // Backpropagates the current node to all the roots
    // Calling get_grad() on a parent node will return
    //      ∂(this node)/∂(parent node)
    void backpropagate() {
        // You expected a function,
        // but it was me, a wrapper!
        // Wrrryyyyy
        this->backpropagate(1);
    }

    void backpropagate(int incoming_grad) {
        this->gradient+=incoming_grad;

        // I'll handle all the supported operation by
        // hand for now
        switch(this->op) {
            case Value::SUM:
                this->parents[0]->backpropagate(incoming_grad);
                this->parents[1]->backpropagate(incoming_grad);
                break;

            case Value::SUB:
                this->parents[0]->backpropagate( -1*incoming_grad);
                this->parents[1]->backpropagate( -1*incoming_grad);
                break;

            case Value::MUL:
                this->parents[0]->backpropagate(incoming_grad * parents[1]->data);
                this->parents[1]->backpropagate(incoming_grad * parents[0]->data);
                break;
            
            case Value::DIV:
                this->parents[0]->backpropagate(incoming_grad / parents[1]->data );
                this->parents[1]->backpropagate(incoming_grad * parents[0]->data/(parents[0]->data * parents[0]->data));
                break;
            
            case Value::POW:
                this->parents[0]->backpropagate(incoming_grad * this->exponent * std::pow(parents[0]->data, this->exponent-1));
                break;

            case Value::INIT:
                // If the value is initialized by hand stop
                break;
        }
    }

    // 
    double get_grad() {
        return this->gradient;
    }

    // Operators Overload
    Value operator+(Value& value) {
        double result_data = this->data + value.data;
        
        // Generate parents
        int n_parents_arg = 2;
        Value** parents_arg = new Value*[n_parents_arg];
        parents_arg[0] = this;
        parents_arg[1] = &value;

        Value result(result_data, parents_arg, n_parents_arg, Value::SUM);

        return result;
    }
    
    Value operator-(Value& value) {
        double result_data = this->data - value.data;

        // Generate parents
        int n_parents_arg = 2;
        Value** parents_arg = new Value*[n_parents_arg];
        parents_arg[0] = this;
        parents_arg[1] = &value;

        Value result(result_data, parents_arg, n_parents_arg, Value::SUB);

        return result;
    }

    Value operator*(Value& value) {
        double result_data = this->data * value.data;
        
        // Generate parents
        int n_parents_arg = 2;
        Value** parents_arg = new Value*[n_parents_arg];
        parents_arg[0] = this;
        parents_arg[1] = &value;

        Value result(result_data, parents_arg, n_parents_arg, Value::MUL);

        return result;
    }

    Value operator/(Value& value) {
        double result_data = this->data / value.data;
        
        // Generate parents
        int n_parents_arg = 2;
        Value** parents_arg = new Value*[n_parents_arg];
        parents_arg[0] = this;
        parents_arg[1] = &value;

        Value result(result_data, parents_arg, n_parents_arg, Value::DIV);

        return result;
    }

    Value pow(double exponent_arg) {
        double result_data = std::pow(this->data, exponent_arg);
        
        // Generate parents
        int n_parents_arg = 1;
        Value** parents_arg = new Value*[n_parents_arg];
        parents_arg[0] = this;

        Value result(result_data, parents_arg, n_parents_arg, Value::POW);
        result.exponent = exponent_arg;

        return result;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Value& value);


    // Graph view      
    std::string to_graph() {
        std::string mermaid_graph = "**Visualize the graph at http://mermaid.live**\n\n";
        mermaid_graph += "flowchart TD\n";

        this->to_graph(mermaid_graph, 0, -1);

        return mermaid_graph;
    }

    // return: last integer used so that I can keep unique labels
    int to_graph(std::string &mermaid_graph, int child_idx, int current_idx) {        
        // Generate current node with its value
        int node_idx = current_idx + 1;
        std::string node_idx_str = std::to_string(node_idx);
        mermaid_graph += "\t"+node_idx_str+"["+std::to_string(this->data)+"]\n";

        // if this is NOT the first connect it to its child (which is an operation node)
        if(current_idx != -1) {
            mermaid_graph += "\t"+node_idx_str+"-->"+std::to_string(child_idx)+"\n";
        }

        // Generate the parent block (which is an operation block) by parsing for correct operator symbol
        int operation_idx = node_idx + 1;
        std::string operation_idx_str = std::to_string(operation_idx);
        switch(this->op) {
            case Value::SUM:
                mermaid_graph +="\t"+operation_idx_str+"{+}\n";
                mermaid_graph +="\t"+operation_idx_str+"-->"+node_idx_str+"\n";
                break;

            case Value::SUB:
                mermaid_graph +="\t"+operation_idx_str+"{-}\n";
                mermaid_graph +="\t"+operation_idx_str+"-->"+node_idx_str+"\n";
                break;

            case Value::MUL:
                mermaid_graph +="\t"+operation_idx_str+"{*}\n";
                mermaid_graph +="\t"+operation_idx_str+"-->"+node_idx_str+"\n";
                break;
            
            case Value::DIV:
                mermaid_graph +="\t"+operation_idx_str+"{/}\n";
                mermaid_graph +="\t"+operation_idx_str+"-->"+node_idx_str+"\n";
                break;
            
            case Value::POW:
                mermaid_graph +="\t"+operation_idx_str+"{^"+std::to_string(this->exponent)+"}\n";
                mermaid_graph +="\t"+operation_idx_str+"-->"+node_idx_str+"\n";
                break;

            case Value::INIT:
                // If the value is initialized by hand it isn't generated by an operation
                operation_idx = node_idx;
                break;
        }

        // Now generate the nodes for all the parents and connect them to the current child (operation block)
        current_idx = operation_idx;
        for(int j=0; j<this->n_parents; j++) {
            current_idx = this->parents[j]->to_graph(mermaid_graph, operation_idx, current_idx);
        }

        return current_idx;
    }

};


std::ostream& operator<<(std::ostream& os, const Value& value) {
    os << value.data;
    return os;
}




    