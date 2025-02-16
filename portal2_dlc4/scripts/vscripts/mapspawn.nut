if (!("Entities" in this)) return;
IncludeScript("ppmod");

::fakeHoldPosPrev <- Vector();
::fakeHold <- null;

::raySolidEnts <- [
  "prop_weighted_cube",
  "prop_monster_box",
  "npc_personality_core",
  "prop_physics"
];

ppmod.onauto(function () {

  local pplayer = ppmod.player(GetPlayer());

  ppmod.fire(["models/elevator/elevator_b.mdl"], "Kill");

  ppmod.interval(function ():(pplayer) {

    for (local i = 0; i < raySolidEnts.len(); i ++) {
      ppmod.forent(raySolidEnts[i], function (cube) {
        cube.moveType = 8;
        cube.solid = 2;

        if (cube == fakeHold) return;

        local scope = cube.GetScriptScope();
        if (!("_vel" in scope)) {
          scope._vel <- Vector();
          scope.InputUse <- function () {
            if (fakeHold) {
              fakeHold = null;
              fakeHoldPosPrev = Vector();
            } else fakeHold = self;
            return false;
          };
        }

        local solidEnts = [];
        for (local i = 0; i < raySolidEnts.len(); i ++) {
          ppmod.forent(raySolidEnts[i], function (ent):(cube, solidEnts) {
            if (cube == ent) return;
            solidEnts.push(ent);
          });
        }

        local pos = cube.GetOrigin();
        local ray1 = ppmod.ray(pos, pos - Vector(0, 0, 18), solidEnts);

        local speed2d = scope._vel.Length2D();
        local vel2d = scope._vel.Copy().Normalize2D();
        if (speed2d > 1.0 && ppmod.ray(pos, pos + vel2d * 18, solidEnts).fraction != 1.0){
          scope._vel -= vel2d * speed2d * 1.1;
        }

        if (ray1.fraction == 1.0) {
          scope._vel -= Vector(0, 0, 0.667);
        } else {
          scope._vel.z = 0.0;
          scope._vel *= 0.4;
        }

        cube.SetAbsOrigin(pos + scope._vel);

      });
    }

    if (!fakeHold) return;

    local fvec = pplayer.eyes.GetForwardVector();
    local pos = pplayer.eyes.GetOrigin() + fvec * 64.0;

    fakeHold.SetAbsOrigin(pos);
    fakeHold.SetForwardVector(fvec);

    local scope = fakeHold.GetScriptScope();
    scope._vel = pos - fakeHoldPosPrev;
    local len = scope._vel.Norm();
    scope._vel = scope._vel * min(fabs(len), 15.0) * (len < 0.0 ? -1 : 1);

    ::fakeHoldPosPrev = pos;

  });

});
