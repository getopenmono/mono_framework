//
//  matrix_interface.h
//  mono
//
//  Created by Kristoffer Lyder Andersen on 07/10/15.
//
//

#ifndef matrix_interface_h
#define matrix_interface_h


namespace mono { namespace math {
    
    template <int cols, int rows, typename numberType>
    class IMatrix {
        
        
    public:
        
        /**
         * Construct the identity matrix
         */
        IMatrix();
        
        /**
         * Construct a matrix with predefined content
         *
         */
        IMatrix(numberType *dataArray);
        
        virtual IMatrix operator*(IMatrix<cols, rows, numberType> &other) = 0;

        
    };
    
} }


#endif /* matrix_interface_h */
