/*
 * An example morph::Visual scene, containing a CartGrid.
 */

#include <iostream>
#include <vector>
#include <cmath>

#include <morph/Scale.h>
#include <morph/Vector.h>
#include <morph/Visual.h>
#include <morph/VisualDataModel.h>
#include <morph/CartGridVisual.h>
#include <morph/CartGrid.h>

#include <armadillo>

using namespace arma;

int main()
{
    // Parameters
    int n = 30;
    double scale = 1.0;
    double alpha = 1.0;
    double time_start = 0.0;
    double time_end = 0.1;
    double dt = 0.00001;

    // Derived
    double h = scale / static_cast<double>(n+1);
    dmat T(n + 2, n + 2, fill::zeros);
    dmat T_old = T;

    for (double t = time_start; t <= time_end; t += dt)
    {
        //      BOUNDARY CONDITIONS
        // Left and right walls
        T.col(0).fill(100.0);
        T.col(n + 1).fill(0.0);

        // Top and bottom walls
        T.row(n + 1).fill(100.0);
        T.row(0) = T.row(1);

        T_old = T;

        for (size_t i = 1; i < n+1; i++)
        {
            for (size_t j = 1; j < n + 1; j++)
            {
                T(i, j) = T_old(i, j) + alpha * dt / (h * h) *
                    ( T_old(i + 1, j) - 2 * T_old(i, j) + T_old(i - 1, j)
                    + T_old(i, j + 1) - 2 * T_old(i, j) + T_old(i, j - 1));
            }
        }

        std::cout << "Time: " << dt << endl;
    }

    std::cout.precision(6);
    std::cout << ios::fixed;

    std::cout << T << endl;

    //          SCENE SETUP
    // Contructor args are width, height, title, coordinate arrows offset, coordinate
    // arrows lengths, coord arrow thickness, ?
    morph::Visual v(1600, 1000, "morph::CartGridVisual", { -0.8,-0.8 }, { .05,.05,.05 }, 2.0f, 0.0f);
    // You can set a field of view (in degrees)
    v.fov = 15;
    // Should the scene be 'locked' so that movements and rotations are prevented?
    v.sceneLocked = false;
    // Whole-scene offsetting uses two methods - this one to set the depth at which the object is drawn
    v.setZDefault(-5.0f);
    // ...and this one to set the x/y offset. Try pressing 'z' in the app window to see what the current sceneTrans is
    v.setSceneTransXY(0.0f, 0.0f);
    // Make this larger to "scroll in and out of the image" faster
    v.scenetrans_stepsize = 0.5;
    // The coordinate arrows can be hidden
    v.showCoordArrows = false;
    // The title can be hidden
    v.showTitle = false;
    // The coord arrows can be displayed within the scene (rather than in, say, the corner)
    v.coordArrowsInScene = false;
    // You can set the background (white, black, or any other colour)
    v.backgroundWhite();
    // You can switch on the "lighting shader" which puts diffuse light into the scene
    v.lightingEffects();
    // Add some text labels to the scene
    v.addLabel("This is a\nmorph::CartGridVisual\nobject", { 0.26f, -0.16f, 0.0f });


    //             MESH SETUP   
    // Create a HexGrid to show in the scene
    morph::CartGrid cg(h, h, 0.0, 0.0, 1.0, 1.0);
    std::cout << "Number of pixels in grid:" << cg.num() << std::endl;

    // *NB* This call (or any other 'set boundary' call) is essential, as it sets up the
    // d_ vectors in the CartGrid. Without it, the CartGrid will be unusable!
    cg.setBoundaryOnOuterEdge();

    auto test = cg.getCoordinates3();
    morph::vVector<morph::Vector<float, 3>> points((n+2)*(n+2));
    morph::vVector<float> data((n + 2) * (n + 2));
    for (int j = 0; j < n+2; j++) {
        for (int i = 0; i < n+2; i++) {
            float x = h * i;
            float y = h * j;
            float z = T(i, j)/100.0;
            //points[((n + 2) * (n + 2) - (n + 2)) + i - j * (n + 2)] = { x, y, z };
            data[((n + 2) * (n + 2) - (n + 2)) + i - j * (n + 2)] = z;
        }
    }

    // Add a CartGridVisual to display the CartGrid within the morph::Visual scene
    morph::Vector<float, 3> offset = { 0.0f, -0.05f, 0.0f };
    morph::CartGridVisual<float>* cgv = new morph::CartGridVisual<float>(v.shaderprog, v.tshaderprog, &cg, offset);
    cgv->cartVisMode = morph::CartVisMode::Triangles;
    cgv->setScalarData(&data);
    cgv->cm.setType(morph::ColourMapType::Twilight);
    cgv->finalize();
    unsigned int gridId = v.addVisualModel(cgv);
    std::cout << "Added CartGridVisual with gridId " << gridId << std::endl;

    while (v.readyToFinish == false) {
        glfwWaitEventsTimeout(0.018);
        v.render();
    }

    return 0;
}
