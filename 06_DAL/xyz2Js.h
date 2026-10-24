

void angulos(float &j_1, float &j_2, float &j_3, float x_x, float y_y, float z_z) {


	int cd = 1;
	int a = j_2; int b = j_3;
	//if((a < 0) && (b < 0)) cd = 1; // <- no se usa
	if((a > 0) && (b < 0)) cd = 1; //
	else if((a > 0) && (b > 0)) cd = 1; //
	//else if((a < 0) && (b > 0)) cd = -1; // <- no se usa

	float p[3] = {x_x,y_y,z_z};
	p[2] -= 145;

	float l1[3] = {145,110,100};
    float r, t, temp;
    float q1, q2, q3;
    float x1, y1, z1;
    //float lx;
    float ly, lz;

    x1 = p[0] * p[0]; y1 = p[1] * p[1]; z1 = p[2] * p[2];
    //lx = l1[0] * l1[0];
    ly = l1[1] * l1[1]; lz = l1[2] * l1[2];

    r = (180/pi);

    q1 = atan2(p[1],p[0]) * r;
    j_1 = q1;

    temp = x1 + y1 + z1 - ly - lz;
    temp = temp/(2*l1[1]*l1[2]);
    t = temp * temp;

    q3 = atan2(cd*sqrt(1-t),temp)*r;
    j_3 = q3;

    q2 = atan2(p[2],(-cd*sqrt(x1+y1)))*r;
    //q2 = atan2(p[2],(-1*sqrt(x1+y1)))*r;
    q2 = q2-atan2(l1[2]*sin(q3*pi/180),l1[1]+l1[2]*cos(q3*pi/180))*r - 90;
    j_2 = q2;

}
