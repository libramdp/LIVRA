// Ejemplo de segmentacion y obtencion de descriptores

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

int main() {

// nombre de la imagen que se va a cargar y manipular como matriz
	char NombreImagen[] = "muestras.jpg";  

// definimos los nombres de todas las matrices que alojaran imagenes a lo largo de todo el proceso
	
	Mat img, img_gris, umbralizada_1, umbralizada_2, umbralizada_3, scr_neg, dst1, fuente, dst2; 

// cargamos la imagen original

	img = imread(NombreImagen);
	if (!img.data) {
		cout << "Error al cargar la imagen: " << NombreImagen << endl;
		system("PAUSE");
		exit(1);
	}
// si no hay error de carga, pasamos la imagen a escala de grises
	
	cvtColor(img, img_gris, CV_BGR2GRAY); // pasamos a gris

// mostramos en pantalla la imegen colo y en gris

	namedWindow("Color", CV_WINDOW_AUTOSIZE);
	imshow("Color", img);
	namedWindow("Gris", CV_WINDOW_AUTOSIZE);
	imshow("Gris", img_gris);
// guardamos en la carpeta del proyecto la imagen en gris como jpg.
	
	imwrite("Gris.jpg", img_gris);

// prueba de realce de bordes 
/*
// Creamos una matriz para alojar la imagen filtrada que debe ser del mismo tamaño que la imagen original y 
// con los pixeles de 8 bits sin signo, pues la transformacion puede dar un resultado con un noveno bit
	
	Mat img_gris_realce(img_gris.cols, img_gris.rows, CV_8U);

// Creamos la matriz de 3x3 para alojar el kernel
	Mat Kernel(3, 3, CV_8S);

// preparamos tres opciones de kernel
/*
	Kernel.at<char>(0) = -1; Kernel.at<char>(1) = -1; Kernel.at<char>(2) = -1;
	Kernel.at<char>(3) = -1; Kernel.at<char>(4) = 9; Kernel.at<char>(5) = -1;
	Kernel.at<char>(6) = -1; Kernel.at<char>(7) = -1; Kernel.at<char>(8) = -1;
*/
/*
	Kernel.at<char>(0) = 0; Kernel.at<char>(1) = -1; Kernel.at<char>(2) = 0;
	Kernel.at<char>(3) = -1; Kernel.at<char>(4) = 5; Kernel.at<char>(5) = -1;
	Kernel.at<char>(6) = 0; Kernel.at<char>(7) = -1; Kernel.at<char>(8) = 0;
*/
/*
	Kernel.at<char>(0) = -1; Kernel.at<char>(1) = -2; Kernel.at<char>(2) = -1;
	Kernel.at<char>(3) = -2; Kernel.at<char>(4) = 13; Kernel.at<char>(5) = -2;
	Kernel.at<char>(6) = -1; Kernel.at<char>(7) = -2; Kernel.at<char>(8) = -1;

*/
/*
	filter2D(img_gris, img_gris_realce, CV_8U, Kernel);
	
	namedWindow("Gris-realzada", CV_WINDOW_AUTOSIZE);
	imshow("Gris-realzada", img_gris_realce);
	//imwrite("Gris-realzada.jpg", img_gris_realce);

/*
*/

// continuamos con la segmentacion por umbralizacion

	threshold(img_gris, umbralizada_1, 100, 255, CV_THRESH_BINARY); // umbralizamos
	threshold(img_gris, umbralizada_2, 180, 255, CV_THRESH_BINARY); // umbralizamos
	threshold(img_gris, umbralizada_3, 150, 255, CV_THRESH_BINARY); // umbralizamos
	
// obtenemos la imagen inverza de la umbralizada-150

	bitwise_not(umbralizada_3, fuente); // la nueva imagen de trabajo se llama fuente

// transformaciones morfologicas 
	Mat kernel;
	kernel = cv::getStructuringElement(MORPH_CROSS, Size(3,3)); // forma del elemnto estructurante :// MORPH_RECT / MORPH_RECT  /  MORPH_CROSS/ MORPH_ELLIPSE

	dilate(fuente, dst1, kernel, Point(-1, -1), 3); // tres dilataciones para llenar huecos
			
    erode(dst1, dst2, kernel, Point(-1, -1), 3); // tres erociones para volvel al tamaño original

// dst2 es la nueva imagen fuente correctamente segmentada  

// encontramos y mostramos los contornos  

  vector<vector<Point> > contornos;
  Mat contourOuput = dst2.clone();

  findContours(contourOuput, contornos, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);


  //cout << endl;
  /*
  cout << "El numero de contornos encontrados es:" << contornos.size() << endl;
  cout << "La cantidad de puntos de vector 0 es:" << contornos[0].size() << endl;
  cout << "La cantidad de puntos de vector 1 es:" << contornos[1].size() << endl;
  cout << "La cantidad de puntos de vector 2 es:" << contornos[2].size() << endl;
  cout << "La cantidad de puntos de vector 3 es:" << contornos[3].size() << endl;
  cout << "La cantidad de puntos de vector 4 es:" << contornos[4].size() << endl;
  cout << "La cantidad de puntos de vector 5 es:" << contornos[5].size() << endl;

*/
//cout << "El tamaño del vector 5 es: " << contornos[5].size() << endl;
// cout << "El contenido del vector 5 es: " << contornos[5] << endl;

// Dibujamos los contornos, primero cremoa una imagen vacia
  Mat contourImagen(dst2.size(), CV_8UC3, Scalar(255,255,255));

   Scalar colors[3];
  colors[0] = Scalar(255, 0, 0);
  //colors[1] = Scalar(0, 255, 0);
  //colors[2] = Scalar(0, 0, 255);

   
  for (size_t idx = 0; idx < contornos.size(); idx++)
	  //drawContours(contourImagen, contornos, idx, colors[idx % 3]);
  
	  drawContours(contourImagen, contornos, idx, colors[0]);
       
      imshow("vercontornos", contourImagen);
      imwrite("vercontornos.jpg", contourImagen);

	  float perimetro, area;
	  int k=0;
	  Mat descriptores(contornos.size(), int (2), CV_32FC1);
	  	  for (size_t idx = 0; idx < contornos.size(); idx++) {
		  area = contourArea(contornos[idx], false);
		  perimetro = arcLength(contornos[idx], 1);

		  descriptores.at<float>(k, 0) = perimetro;
		  descriptores.at<float>(k, 1) = area;
		 
		  k++;
	  }
	  cout << "Matriz descriptores: " << endl;
	  cout << "Dimension matriz; " << descriptores.size() << endl;
	  cout << descriptores << endl;


//	namedWindow("Umbral-100", CV_WINDOW_AUTOSIZE);
	//imshow("Umbral-100", umbralizada_1);
	
	//namedWindow("Umbral-180", CV_WINDOW_AUTOSIZE);
	//imshow("Umbral-180", umbralizada_2);

	namedWindow("Umbral-150", CV_WINDOW_AUTOSIZE);
	imshow("Umbral-150", umbralizada_3);

	namedWindow("fuente", CV_WINDOW_AUTOSIZE);
	imshow("fuente", fuente);
	imwrite("fuente.jpg", fuente);


	namedWindow("dilatada", CV_WINDOW_AUTOSIZE);
	imshow("dilatada", dst1);
	imwrite("dilatada.jpg", dst1);
	namedWindow("erocionada", CV_WINDOW_AUTOSIZE);
	imshow("erocionada", dst2);
	imwrite("erocionada.jpg", dst2);
//	imwrite("Umbral-150.jpg", umbralizada_3);
//	imwrite("Umbral-180.jpg", umbralizada_2);
//	imwrite("Umbral-100.jpg", umbralizada_1);


	//namedWindow("neg", CV_WINDOW_AUTOSIZE);
//	imshow("neg", scr_neg);


	cvWaitKey(0);
	return(0);
}

