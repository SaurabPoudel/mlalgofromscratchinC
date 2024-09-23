#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LEARNING_RATE 0.01f
#define EPOCHS 20
#define BATCH_SIZE 64

typedef struct{
	float *weight, bias;
	int input_size;
}LogisticRegression;

typedef struct{
	float** data;
	int* labels;
	int nSamples, nFeatures;
}Dataset;

void init_model(LogisticRegression* model, int input_size){
	
	model->input_size = input_size;
	model->weight = malloc(input_size * sizeof(float));
	model->bias = 0;

	for(int i=0; i < input_size; i++){
		model->weight[i] = ((float)rand() / RAND_MAX - 0.5f) * 0.01f;
	}
}

float sigmoid(float z){
	return 1.0f / (1.0f + expf(-z));
}

void train(LogisticRegression* model, float* input, int label, float lr){

	float linear_output = model->bias;

	for(int i=0; i < model->input_size; i++){
		linear_output += model->weight[i] * input[i];
	}

	float prediction = sigmoid(linear_output);
	float error = label - prediction;

	for (int i = 0; i < model->input_size; i++) {
		model->weight[i] += lr * error * input[i];
	}

	model->bias += lr * error;
}

int predict(LogisticRegression *model, float *input){

	float linear_output = model->bias;
	
	for(int i=0; i < model->input_size; i++){
		linear_output += model->weight[i] * input[i];
	}

	return sigmoid(linear_output) >= 0.5;
}

Dataset read_csv(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        exit(1);
    }

    Dataset dataset;
    dataset.nSamples = 0;
    dataset.nFeatures = 0;

    char line[1024];
    fgets(line, sizeof(line), file); // Skip the header

    while (fgets(line, sizeof(line), file)) {
        dataset.nSamples++;
        char *token = strtok(line, ",");
        int features_count = 0;

        while (token) {
            if (dataset.nSamples == 1) {
                dataset.nFeatures++;
            }
            features_count++;
            token = strtok(NULL, ",");
        }
    }

    fseek(file, 0, SEEK_SET);
    fgets(line, sizeof(line), file); // Read header again

    dataset.data = malloc(dataset.nSamples * sizeof(float*));
    dataset.labels = malloc(dataset.nSamples * sizeof(int));

    for (int i = 0; i < dataset.nSamples; i++) {
        dataset.data[i] = malloc(dataset.nFeatures * sizeof(float));
        fgets(line, sizeof(line), file);
        char *token = strtok(line, ",");
        for (int j = 0; j < dataset.nFeatures; j++) {
            if (j < dataset.nFeatures - 1) {
                dataset.data[i][j] = atof(token);
                token = strtok(NULL, ",");
            } else {
                dataset.labels[i] = atoi(token);
            }
        }
    }

    fclose(file);
    return dataset;
}

	

void free_dataset(Dataset* dataset){

	for(int i = 0; i < dataset->nSamples; i++){
		free(dataset->data[i]);
	}

	free(dataset->data);
	free(dataset->labels);
}

int main(){
	
	LogisticRegression model;
	Dataset dataset = read_csv("data/refined_titanic_data.csv");

	srand(time(NULL));

	init_model(&model, dataset.nFeatures - 1);
	for(int epoch = 0; epoch < EPOCHS; epoch++){
		for(int i = 0; i < dataset.nSamples; i += BATCH_SIZE){
			for(int j = 0; j < BATCH_SIZE && i + j < dataset.nSamples; j++){
				int idx = i + j;
				train(&model, dataset.data[idx], dataset.labels[idx], LEARNING_RATE);
			}
		}
		int correct = 0;
		for(int i=0; i < dataset.nSamples; i++){
			if( predict(&model, dataset.data[i]) == dataset.labels[i]){
			correct++;
			}
		}

			printf("Epoch %d, Accuracy: %.2f%%\n", epoch + 1, (float)correct / dataset.nSamples * 100);
	}

	free(model.weight);
	free_dataset(&dataset);

	return 0;
	
}

