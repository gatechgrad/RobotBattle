import Blender


## Filename to store exported data
filename="object.c"
## Which object to export
object="Cube"

### NO NEED TO CHANGE ANYTHING BELOW HERE
oldColor="mmkay..."
obj = Blender.Object.Get(object)
nmesh = Blender.NMesh.GetRaw(obj.data.name)
name =  obj.data.name
type = obj.data.block_type

## Open file
f = open(filename,"w")

print "File %s opened. Now exporting data..." % filename

#print "void loadModel() {"
f.write("void loadModel() {\n");
for face in nmesh.faces:
  vn=0
  #print "glBegin(GL_TRIANGLES);"
  f.write("glBegin(GL_TRIANGLES);\n")
  for vertex in face.v:
    ## Build glVertex3f
    data = "  glVertex3f("
    data = data + "%s," % vertex.co[0]
    data = data + "%s," % vertex.co[1]
    data = data + "%s);\n" % vertex.co[2]
    ## Build glNormal3f
    normal = "  glNormal3f("
    normal = normal + "%s," % vertex.no[0]
    normal = normal + "%s," % vertex.no[0]
    normal = normal + "%s);\n" % vertex.no[2]
    ## Build color changing command
    color = "  setAmbient(%s," % face.col[vn].r
    color = color + "%s," % face.col[vn].g
    color = color + "%s);\n" % face.col[vn].b
    ## Output
    ## Color value is outputted only if necessary
    if oldColor != color:
      #print color
      f.write(color)
    ## Normal info
    f.write(normal)
    #print normal
    ## Vertex info
    f.write(data)
    #print data
    vn=vn+1
    oldColor = color
  #print "glEnd();"
  f.write("glEnd();\n")

#print "}"
f.write("}\n")
print "Export complete"

f.close()