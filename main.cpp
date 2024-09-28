#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include <iomanip>
#include <cassert>

using namespace std;


vector<sf::Color> colors;

sf::Color MandelbrotCPUShader(long double x, long double y) { // pixel shader
    complex<long double> c(x, y);
    complex<long double> z(0, 0);
    int breakPoint = 255;
    for (int step = 0; step < 256; step++) {
        z = z * z + c;
        if (norm(z) > 4) {
            breakPoint = step;
            break;
        }
    }
    assert(0 <= breakPoint && breakPoint < (int)colors.size());
    return colors[breakPoint];
}

//
//sf::Color MandelbrotCPUShader(long double x, long double y) { // pixel shader
//    complex<long double> c(x, y);
//    complex<long double> z(-y, x);
//    int bp = 255;
//    for (int step = 0; step < 256; step++) {
//        z = z * z + c;
//        if (norm(z) > 4) {
//            bp = step;
//            break;
//        }
//    }
//    assert(0 <= bp && bp < (int)colors.size());
//    return colors[bp];
//}

const int DIM = 1 << 10;
sf::Image img;

long double xc, yc, sz;

void func(pair<int, int> it) {
    long double x1 = yc;
    long double y1 = xc;
    long double dim = sz;
    x1 -= dim;
    y1 -= dim;
    dim *= 2;
    for (int j = it.first; j <= it.second; j++) {
        for (int i = 0; i < img.getSize().x; i++) {

            long double rap_i = (i + 0.5f) / DIM;
            long double rap_j = (j + 0.5f) / DIM;

            long double y = x1 + dim * rap_j;
            long double x = y1 + dim * rap_i;

            img.setPixel(i, j, MandelbrotCPUShader(x, y));
        }
    }
}


float solve() {
    sf::Clock t;
    const int SPLIT_FACTOR = 20;
    vector<unique_ptr<sf::Thread>> th;
    vector<int> pivots(SPLIT_FACTOR + 1);
    for (int i = 0; i <= SPLIT_FACTOR; i++) {
        pivots[i] = DIM * i / SPLIT_FACTOR;
    }
    assert(pivots[0] == 0);
    assert(pivots.back() == DIM);
    for (int i = 0; i < SPLIT_FACTOR; i++) {
        th.push_back(unique_ptr<sf::Thread>(new sf::Thread(&func, (make_pair(pivots[i], pivots[i + 1] - 1)))));
    }
    for (auto& it : th) {
        it->launch();
    }
    for (auto& it : th) {
        it->wait();
    }
    return t.getElapsedTime().asSeconds();
}

int main() {
    if (!sf::Shader::isAvailable())
    {
        cout << "shaders are not available... (sad)!\n";
        // shaders are not available...
    }
    else 
    {
        cout << "shaders are available... (happy)!\n";
    }

    sf::Shader shader;
    // load only the fragment shader
    if (!shader.loadFromFile("Mandelbrot.frag", sf::Shader::Fragment))
    {
        cout << "Could not load \"Mandelbrot.frag\"(sad)\n";
        // error...
    }
    else
    {
        cout << "load succesfull: \"Mandelbrot.frag\"(happy)\n";
    }

    shader.setUniform("texture", sf::Shader::CurrentTexture);


    sf::Font font;
 
    if (!font.loadFromFile("font.ttf")) {
        cout << "Couldn't load font!\n";
        exit(0);
    }

    {
        for (int i = 0; i < 256; i++) {
            colors.push_back(sf::Color((i + 32333) % 256, i, (i + 3234246) % 256));
        }
    }
    float ela = 0;
    xc = -0.77568377;
    yc = 0.13646737;

     //xc = -0.10109636384562;
     //yc = 0.95628651080914;
     //swap(xc, yc);
    //xc = 2.613577e-1;
    //yc = -2.018128e-3;
    sz = 4;
    img.create(DIM, DIM);
    solve();

    sf::RenderWindow window(sf::VideoMode(900, 900), "Trippy!");

    sf::Clock frameClock;

    sf::Texture txt;

    sf::RectangleShape rectFundal;
    rectFundal.setFillColor(sf::Color::Yellow);
    rectFundal.setPosition(sf::Vector2f(-0.5, -0.5));
    rectFundal.setSize(sf::Vector2f(1, 1));

    if (!txt.loadFromImage(img)) {
        for (int i = 1; i <= 10; i++) {
            cout << "fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        }
    }

    sf::RectangleShape shp;
    sf::View view;

    view.setSize(sf::Vector2f(1, 1));
    view.setCenter(sf::Vector2f(0, 0));

    window.setView(view);

    shp.setTexture(&txt);
    shp.setSize(sf::Vector2f(1, 1) * 0.7f);
    shp.setPosition(sf::Vector2f(0, 0) - shp.getSize() / 2.0f);

    long double dim = 2;

    sf::Clock glock;

    sf::Text text;

    sf::VertexArray varray1(sf::LinesStrip, 2);
    varray1[0].position = sf::Vector2f(-1, -1) * 0.01f;
    varray1[1].position = sf::Vector2f(+1, +1) * 0.01f;


    sf::VertexArray varray2(sf::LinesStrip, 2);
    varray2[0].position = sf::Vector2f(-1, +1) * 0.01f;
    varray2[1].position = sf::Vector2f(+1, -1) * 0.01f;

    for (int i = 0; i < 2; i++) {
        varray1[i].color = sf::Color(sf::Color(200, 20, 70));
        varray2[i].color = sf::Color(sf::Color(200, 20, 70));
    }

    sf::VertexArray geometriePentruShader(sf::Quads, 4);

    geometriePentruShader[0].texCoords = sf::Vector2f(xc, yc) + sf::Vector2f(-sz, -sz) / 2.0f;
    geometriePentruShader[1].texCoords = sf::Vector2f(xc, yc) + sf::Vector2f(-sz, +sz) / 2.0f;
    geometriePentruShader[2].texCoords = sf::Vector2f(xc, yc) + sf::Vector2f(+sz, +sz) / 2.0f;
    geometriePentruShader[3].texCoords = sf::Vector2f(xc, yc) + sf::Vector2f(+sz, -sz) / 2.0f;
   

    //geometriePentruShader[0].texCoords = sf::Vector2f(0, 0);
    //geometriePentruShader[1].texCoords = sf::Vector2f(0, 1);
    //geometriePentruShader[2].texCoords = sf::Vector2f(1, 1);
    //geometriePentruShader[3].texCoords = sf::Vector2f(1,0);

    sf::FloatRect bounds = shp.getGlobalBounds();
    float left = bounds.left;
    float right = bounds.left + bounds.width;
    float top = bounds.top;
    float bottom = bounds.top + bounds.height;
    
    geometriePentruShader[0].position = sf::Vector2f(left, top);
    geometriePentruShader[1].position = sf::Vector2f(left, bottom);
    geometriePentruShader[2].position = sf::Vector2f(right, bottom);
    geometriePentruShader[3].position = sf::Vector2f(right, top);

    //geometriePentruShader[0].color = sf::Color::Red;
    //geometriePentruShader[1].color = sf::Color::Green;
    //geometriePentruShader[2].color = sf::Color::Blue;
    //geometriePentruShader[3].color = sf::Color::Yellow;

    sf::Clock timeCeas;

    sf::Clock altCeas;

    bool pressUp = 0, pressDown = 0, pressLeft = 0, pressRight = 0;
    while (window.isOpen()) {

        float dt = frameClock.restart().asSeconds();
        ela += dt;

        const float consume = 0.1;

        bool now_pressUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W));
        bool now_pressDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S));
        bool now_pressLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A));
        bool now_pressRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D));

        bool upd = 0;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                upd = 1;
                sz *= pow(0.9, event.mouseWheelScroll.delta);
            }
        }

        if (ela >= 0.1 && 0) {
            sz *= 0.9;
            upd = 1;
            ela = 0;
        }

        float speed = 0.1 * sz;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
            speed *= 3;
        }

        if (!pressUp && now_pressUp) {
            yc -= speed;
            upd = 1;
        }

        if (!pressDown && now_pressDown) {
            yc += speed;
            upd = 1;
        }


        if (!pressLeft && now_pressLeft) {
            xc -= speed;
            upd = 1;
        }

        if (!pressRight && now_pressRight) {
            xc += speed;
            upd = 1;
        }

        pressUp = now_pressUp;
        pressDown = now_pressDown;
        pressLeft = now_pressLeft;
        pressRight = now_pressRight;

        ela += dt;
        sf::Text text;
        float num = 6.66;
        stringstream ss1;
        ss1 << "(" << fixed << setprecision(8) << xc << ", " << yc << ", " << sz << ")";
        text.setString(ss1.str());
        text.setFont(font);
        text.setScale(sf::Vector2f(1, 1) * 0.001f);
        text.setPosition(sf::Vector2f(-0.5, -0.5));

        if (upd) {
            geometriePentruShader[0].texCoords = sf::Vector2f(xc, yc) + sf::Vector2f(-sz, -sz) / 2.0f;
            geometriePentruShader[1].texCoords = sf::Vector2f(xc, yc) + sf::Vector2f(-sz, +sz) / 2.0f;
            geometriePentruShader[2].texCoords = sf::Vector2f(xc, yc) + sf::Vector2f(+sz, +sz) / 2.0f;
            geometriePentruShader[3].texCoords = sf::Vector2f(xc, yc) + sf::Vector2f(+sz, -sz) / 2.0f;
            //cout << "it took " << fixed << setprecision(6) << " seconds to compute " << t << "\n";
            if (!txt.loadFromImage(img)) {
                for (int i = 1; i <= 10; i++) {
                    cout << "fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                }
            }
            shp.setTexture(&txt);
        }


        window.clear();

        window.draw(rectFundal);
        window.draw(geometriePentruShader, &shader);
        // window.draw(shp, &shader);
        text.setFillColor(sf::Color::Red);
        window.draw(text);
        window.draw(varray1);
        window.draw(varray2);
        //cout << " : " << varray1[0].position.x << " " << varray1[0].position.y << "\n";
        //cout << " : " << varray1[1].position.x << " " << varray1[1].position.y << "\n";
        //cout << "done printing this frame points\n";
        
        sf::Clock u;
        window.display();
        float time_taken_to_draw = u.getElapsedTime().asSeconds();


        shader.setUniform("time", altCeas.getElapsedTime().asSeconds());

        if (timeCeas.getElapsedTime().asSeconds() >= 1)
        {
            cout << "time_taken_to_draw = " << fixed << setprecision(6) << time_taken_to_draw << "\n";
            timeCeas.restart();
        }
    }

    return 0;
}