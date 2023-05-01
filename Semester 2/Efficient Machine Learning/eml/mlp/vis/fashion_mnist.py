import torch
import matplotlib.pyplot as plt

def to_label( i_id ):
    l_labels = [ 'T-Shirt',
                'Trouser',
                'Pullover',
                'Dress',
                'Coat',
                'Sandal',
                'Shirt',
                'Sneaker',
                'Bag',
                'Ankle Boot' ]
    return l_labels[i_id]

def plot( i_off,
          i_stride,
          io_data_loader,
          io_model,
          i_path_to_pdf = None ):
    io_model.eval()

    if ( i_path_to_pdf != None ):
        import matplotlib.backends.backend_pdf
        l_pdf_file = matplotlib.backends.backend_pdf.PdfPages( i_path_to_pdf )
    
    l_counter = 0
    for (l_data, _) in io_data_loader:
        for l_img in l_data:
            if l_counter >= i_off and (l_counter - i_off) % i_stride == 0:
                plt.figure(figsize=(3, 3))
                plt.imshow(l_img.squeeze(), cmap='gray')
                l_prediction = io_model.forward(l_img)
                plt.title(to_label(l_prediction.argmax()))
                if ( i_path_to_pdf != None ): l_pdf_file.savefig()
                plt.close()
            l_counter += 1

    if ( i_path_to_pdf != None ):
        l_pdf_file.close()