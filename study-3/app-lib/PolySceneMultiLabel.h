#include <vector>

#include "PolyString.h"
#include "PolyEntity.h"
#include "PolyLabel.h"
#include "PolySceneLabel.h"
#include "PolyRenderer.h"

namespace Polycode
{

class SceneMultiLabel : public Entity
{
public:

	SceneMultiLabel(String text, double size, double spacing);
	virtual ~SceneMultiLabel();

	void setText(String text);
	String getText();

	double getHeight();
	double getWidth();

private:
	double size;
	double spacing;
	std::vector<SceneLabel*> labels;

	void clearLabels();

};


}






