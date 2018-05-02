import React, { Component } from 'react';
import { connect } from 'react-redux';
import PropTypes from 'prop-types';
import MarkerInfo from './MarkerInfo';
import { traverseTreeWithURI, jsonToLuaTable } from '../../../utils/propertyTreeHelpers';
import { startListening, stopListening } from '../../../api/Actions/index';
import { infoIconKey, StoryKey } from '../../../api/keys';

class Markers extends Component {
  componentDidUpdate() {
    const {
      nodes, screenSpaceProperties, screenVisibilityProperties,
      distFromCamToNodeProperties, screenSpaceRadius,
    } = this.props;

    if (nodes.length > 0) {
      nodes.forEach((node, i) => {
        if (screenSpaceProperties[i].listeners === 0) {
          this.props.StartListening(screenSpaceProperties[i].Description.Identifier);
        }
        if (screenVisibilityProperties[i].listeners === 0) {
          this.props.StartListening(screenVisibilityProperties[i].Description.Identifier);
        }
        if (distFromCamToNodeProperties[i].listeners === 0) {
          this.props.StartListening(distFromCamToNodeProperties[i].Description.Identifier);
        }
        if (screenSpaceRadius[i].listeners === 0) {
          this.props.StartListening(screenSpaceRadius[i].Description.Identifier);
        }
      });
    }
  }

  componentWillUnmount() {
    const {
      nodes, screenSpaceProperties, screenVisibilityProperties, distFromCamToNodeProperties,
    } = this.props;

    nodes.forEach((node, i) => {
      this.props.StopListening(screenSpaceProperties[i].Description.Identifier);
      this.props.StopListening(screenVisibilityProperties[i].Description.Identifier);
      this.props.StopListening(distFromCamToNodeProperties[i].Description.Identifier);
    });
  }

  createInfoMarkers() {
    const {
      nodes, screenSpaceProperties, screenVisibilityProperties,
      distFromCamToNodeProperties, infoIcons, screenSpaceRadius,
    } = this.props;

    return (nodes.map((node, i) => {
      const screenSpacePos = jsonToLuaTable(screenSpaceProperties[i].Value).split(',');
      if (screenVisibilityProperties[i].Value === 'true') {
        // TODO Remove the magic numbers
        const distanceFromCam = (100000000000 / distFromCamToNodeProperties[i].Value);
        const showLabel = distanceFromCam > 0.04;

        const planetRadius = Number(screenSpaceRadius[i].Value);
        let size = planetRadius * 0.1;
        const showInfo = planetRadius > 25;

        if (size >= 3) size = 3;
        if (size <= 1.5) size = 1.5;

        let planetInfo;
        if (infoIcons.data) {
          planetInfo = infoIcons.data.planets.find(planet => planet.planet === node.identifier);
        }

        return (<MarkerInfo
          key={screenSpaceProperties[i].Description.Identifier}
          identifier={node.identifier}
          position={screenSpacePos}
          size={size}
          showInfo={showInfo}
          planetInfo={planetInfo}
          showLabel={showLabel}
          planetRadius={planetRadius}
        />);
      }
    })
    );
  }

  render() {
    return (
      <div className={'Markers'}>
        {this.props.screenSpaceProperties.length > 0 && this.createInfoMarkers()}
      </div>
    );
  }
}

const mapStateToProps = (state) => {
  const sceneType = 'Scene';
  let nodes = [];
  const screenSpaceProperties = [];
  const screenVisibilityProperties = [];
  const distFromCamToNodeProperties = [];
  let infoIcons = {};
  const screenSpaceRadius = [];

  if (Object.keys(state.propertyTree).length !== 0) {
    const storyIdentifierNode = traverseTreeWithURI(state.propertyTree, StoryKey);
    const rootNodes = state.propertyTree.subowners
      .filter(element => element.identifier === sceneType);
    rootNodes.forEach((node) => {
      nodes = [...nodes, ...node.subowners];
    });

    nodes = nodes.filter(node => node.tag.some(tag => tag.includes(storyIdentifierNode.Value)))
      .map(node => Object.assign(node, { key: node.identifier }));

    nodes.forEach((node) => {
      screenSpaceProperties.push(traverseTreeWithURI(state.propertyTree, `Scene.${node.identifier}.ScreenSpacePosition`));
      screenVisibilityProperties.push(traverseTreeWithURI(state.propertyTree, `Scene.${node.identifier}.ScreenVisibility`));
      distFromCamToNodeProperties.push(traverseTreeWithURI(state.propertyTree, `Scene.${node.identifier}.DistanceFromCamToNode`));
      screenSpaceRadius.push(traverseTreeWithURI(state.propertyTree, `Scene.${node.identifier}.ScreenSizeRadius`));
    });
  }

  if (state.fetchData.length > 0) {
    const tmp = (state.fetchData.find(info => info.id === infoIconKey));
    infoIcons = tmp.succeed ? tmp : {};
  }
  return {
    nodes,
    screenSpaceProperties,
    screenVisibilityProperties,
    distFromCamToNodeProperties,
    infoIcons,
    screenSpaceRadius,
  };
};

const mapDispatchToProps = dispatch => ({
  StartListening: (URI) => {
    dispatch(startListening(URI));
  },
  StopListening: (URI) => {
    dispatch(stopListening(URI));
  },
});

Markers = connect(
  mapStateToProps,
  mapDispatchToProps,
)(Markers);

Markers.propTypes = {
  screenSpaceProperties: PropTypes.arrayOf(PropTypes.shape({
    Description: PropTypes.string,
    Value: PropTypes.string,
  })),
  distFromCamToNodeProperties: PropTypes.arrayOf(PropTypes.shape({
    Description: PropTypes.string,
    Value: PropTypes.string,
  })),
  screenVisibilityProperties: PropTypes.arrayOf(PropTypes.shape({
    Description: PropTypes.string,
    Value: PropTypes.string,
  })),
  screenSpaceRadius: PropTypes.arrayOf(PropTypes.shape({
    Description: PropTypes.string,
    Value: PropTypes.string,
  })),
  infoIcons: PropTypes.arrayOf(PropTypes.shape({
    planet: PropTypes.string,
    info: PropTypes.string,
  })),
  nodes: PropTypes.arrayOf(PropTypes.shape({})),
  StartListening: PropTypes.func,
  StopListening: PropTypes.func,
};

Markers.defaultProps = {
  nodes: [],
  screenSpaceProperties: [],
  distFromCamToNodeProperties: [],
  screenVisibilityProperties: [],
  screenSpaceRadius: [],
  infoIcons: [],
  Description: [],
  Value: '',
  StopListening: null,
  StartListening: null,
};

export default Markers;
