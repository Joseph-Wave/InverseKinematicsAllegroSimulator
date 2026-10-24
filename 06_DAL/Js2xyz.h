
#define pi 3.14159265359

class coordinates {
    double m_trans[4*4];
    double j1,j2,j3;
    double l1,l2,l3;
    void roty(double);
    void rotz(double);
    void movz(double);
public:
    coordinates();
    void set_joints(double,double,double);
    void set_lengths(double,double,double);
    double *get_direct();
};

coordinates::coordinates(){

    int i,j;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(i==j){m_trans[i*4+j]=1.0;}
            else{m_trans[i*4+j]=0;}
        }
    }

}
void coordinates::rotz(double ang){
    double m_temp[4*4],m_rotz[4*4]={cos(ang),-sin(ang),0,0,sin(ang),cos(ang),0,0,0,0,1,0,0,0,0,1};
    double temp;
    int i,j,k;
        for (i=0;i<4;i++){
        for(j=0;j<4;j++){
                temp=0;
            for(k=0;k<4;k++){
                temp+=m_trans[i*4+k]*m_rotz[k*4+j];
            }
            m_temp[i*4+j]=temp;
        }
    }
    for(i=0;i<16;i++){m_trans[i]=m_temp[i];};
}
void coordinates::movz(double long_mov){
    double temp,*v_temp,m_mov[4*4]={1,0,0,0, 0,1,0,0, 0,0,1,long_mov,0,0,0,1};
    int i,j,k;
    v_temp=m_trans;
    for (i=0;i<4;i++){
        for (j=0;j<4;j++){
			temp=0;
                for(k=0;k<4;k++){
                        temp+=m_trans[i*4+k]*m_mov[k*4+j];
                }
        }
        v_temp[i*4+3]=temp;
    }
    for(i=0;i<16;i++){m_trans[i]=v_temp[i];}
}
void coordinates::roty(double ang){
    double m_temp[4*4],m_roty[4*4]={cos(ang),0,sin(ang),0,0,1,0,0,-sin(ang),0,cos(ang),0,0,0,0,1};
    double temp;
    int i,j,k;

    for (i=0;i<4;i++){
        for(j=0;j<4;j++){
                temp=0;
            for(k=0;k<4;k++){
                temp+=m_trans[i*4+k]*m_roty[k*4+j];
            }
            m_temp[i*4+j]=temp;
        }
    }
    for(i=0;i<16;i++){m_trans[i]=m_temp[i];};
}
void coordinates::set_joints(double inj1,double inj2,double inj3){
    j1=inj1;
    j2=inj2;
    j3=inj3;
}
void coordinates::set_lengths(double inl1,double inl2,double inl3){
    l1=inl1;
    l2=inl2;
    l3=inl3;
}
double *coordinates::get_direct(){
    rotz(j1);
    movz(l1);
    roty(j2);
    movz(l2);
    roty(j3);
    movz(l3);
    return m_trans;
}

