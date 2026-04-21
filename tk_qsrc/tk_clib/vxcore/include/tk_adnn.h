#define TK_FCC_NN2	RIFF_MAKETAG('N','N','2',' ')

typedef struct TK_AdNn2_Layer_s	TK_AdNn2_Layer;
typedef struct TK_AdNn2_Net_s		TK_AdNn2_Net;

struct TK_AdNn2_Layer_s {
int nn_isz;			//layer input size
int nn_osz;			//layout output size
int nn_stzm;		//stride of zero mask
int nn_szzm;		//size of zero mask
byte nn_afn;		//activation function

int nn_stw4;		//stride with FP4

byte	*wght8;		//neuron weights (FP8)
byte	*wght4;		//neuron weights (FP4)

byte *wght_zm;		//weight zero mask
byte *act_i;		//neuron activations (input)
byte *act_o;		//neuron activations (output size)
};

struct TK_AdNn2_Net_s {
int n_layer;
TK_AdNn2_Layer *layer[16];
};
