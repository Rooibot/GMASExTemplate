# GMCv2 + GMAS + GMCExtended Template Project

There will be a real README here at some point.

## Utilizing as a Template

To use this as a template, find your Unreal Engine install directory for the appropriate version (5.3). It might be something like `C:\Program Files\Epic Games\UE_5.3` or whatever.

In the `UE_5.3` directory, go into the `Templates` directory and clone this git repository recursively. The command you will want to use is:

```
git clone --recursive https://github.com/Rooibot/GMASExTemplate.git
```

The next time you run Unreal Engine 5.3 without a project, you should see a "GMAS Extended" template available; pick this to generate a new C++ project with everything named appropriately.

## Utilizing as a Project

If you want to use this as a project, be aware that it relies on the Character content pack being installed as part of the template use. To install it as a project directly, clone it and then -- from the same `Templates` directory as above, go into `TemplateResources/High/Characters/Content` and copy the contents into a `Content/Characters` directory under this project's root.