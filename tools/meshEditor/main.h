#ifndef MAIN
#define MAIN

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <time.h>
#include <random>
#include <math.h>
#include <vector>

#include <chrono>
using namespace std::chrono;

#include <logo.h>

#include <Display/display.h>
#include <orthoCam.h>

#include <Text/textRenderer.h>

#include <Object/entityComponentSystem.h>

#include <System/sPhysics.h>
#include <System/Rendering/sRender.h>
#include <System/sCollision.h>

#include <World/world.h>
#include <World/marchingWorld.h>
#include <World/tileWorld.h>

#include <Console/console.h>

#include <Debug/collisionMeshDebug.h>

#include <Util/util.h>
#include <log.h>

const int resX = 1000;
const int resY = 1000;
const float MAX_SPEED = 1.0/60.0;

// for smoothing delta numbers
uint8_t frameId = 0;
double deltas[60];

bool debug = false;
bool grid = false;
bool paused = true;

double pointSize = 1.0;
double primitiveSize = 0.5;

std::pair<double, double> activeSite(0.0, 0.0);
double radialTheta = 0.0;

const double deltaPhysics = 1.0/900.0;

using Hop::Object::Component::cTransform;
using Hop::Object::Component::cPhysics;
using Hop::Object::Component::cRenderable;
using Hop::Object::Component::cCollideable;
using Hop::Object::EntityComponentSystem;
using Hop::Object::Id;
using Hop::Object::Component::CollisionPrimitive;

using Hop::System::Rendering::OrthoCam;
using Hop::System::Rendering::Type;
using Hop::System::Rendering::TextRenderer;
using Hop::System::Rendering::sRender;
using Hop::System::Rendering::Shaders;

using Hop::System::Physics::CollisionDetector;
using Hop::System::Physics::CollisionResolver;
using Hop::System::Physics::sPhysics;
using Hop::System::Physics::sCollision;

using Hop::System::Signature;

using Hop::World::MapSource;
using Hop::World::Boundary;
using Hop::World::AbstractWorld;
using Hop::World::TileWorld;
using Hop::Logging::INFO;
using Hop::Logging::WARN;

using Hop::Util::fixedLengthNumber;

#endif /* MAIN */